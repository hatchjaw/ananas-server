#include "DecoderMatrixCoefficientMessenger.h"
#include <AnanasUtils.h>
#include <Eigen/Dense>
#include <cmath>

namespace ananas::HOA
{
    DecoderMatrixCoefficientMessenger::DecoderMatrixCoefficientMessenger(
        juce::AudioProcessorValueTreeState &apvts
    )
        : state(apvts)
    {
        if (!socket.bindToPort(Sockets::DecoderMatrixCoefficientMessengerLocalPort, Utils::Strings::LocalInterfaceIP)) {
            std::cerr << "Secondary Source Messenger failed to bind to port: " << std::strerror(errno) << std::endl;
        }
    }

    void DecoderMatrixCoefficientMessenger::valueTreePropertyChanged(
        juce::ValueTree &treeWhosePropertyHasChanged,
        const juce::Identifier &property
    )
    {
        if (property == Utils::Identifiers::ModulesParamID) {
            speakerPositions.clear();
            sphericalHarmonics.clear();
            for (size_t n{1}; n <= Constants::NumSpeakers; ++n) {
                const auto pos = Cartesian2Spherical(
                    state.getRawParameterValue(Params::getSpeakerCoordinateParamID(n, SpeakerCoordinateAxis::X))->load(),
                    state.getRawParameterValue(Params::getSpeakerCoordinateParamID(n, SpeakerCoordinateAxis::Y))->load(),
                    state.getRawParameterValue(Params::getSpeakerCoordinateParamID(n, SpeakerCoordinateAxis::Z))->load()
                );
                speakerPositions.add(pos);
                std::array<float, Constants::NumChannels> Y{};
                sh_sn3d<Constants::AmbisonicsOrder>(
                    pos.azimuth,
                    pos.elevation,
                    Y
                );
                sphericalHarmonics.add(Y);
            }

            computeDecoderMatrix();

            if (auto *obj = treeWhosePropertyHasChanged[property].getDynamicObject()) {
                for (const auto &prop: obj->getProperties()) {
                    auto module{obj->getProperty(prop.name)};
                    auto hasChanged{module.getProperty(Utils::Identifiers::ModulePositionHasChangedPropertyID, false)};
                    if (hasChanged) {
                        const auto row0{module.getProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID, 0)};
                        const auto row1{module.getProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID, 1)};

                        juce::OSCBundle bundle;

                        // decoderMatrix.row(i) is speaker i's decoder row — push to the device responsible for it
                        std::array<float, Constants::NumChannels> row{};

                        const int idx0{row0};
                        jassert(idx0 >= 0 && idx0 < decoderMatrix.rows());
                        bundle.addElement(juce::OSCMessage{Params::getDecoderRowParamID(0), idx0});
                        Eigen::Map<Eigen::VectorXf>(row.data(), Constants::NumChannels) = decoderMatrix.row(idx0);
                        for (size_t n{0}; n < row.size(); ++n) {
                            DBG("Sending OSC: " << Params::getDecoderCoefficientParamID(0, n) <<
                                " " << juce::String{row[n]} << " to " << prop.name.toString());
                            bundle.addElement(juce::OSCMessage{Params::getDecoderCoefficientParamID(0, n), row[n]});
                        }

                        const int idx1{row1};
                        jassert(idx1 >= 0 && idx1 < decoderMatrix.rows());
                        bundle.addElement(juce::OSCMessage{Params::getDecoderRowParamID(1), idx1});
                        Eigen::Map<Eigen::VectorXf>(row.data(), Constants::NumChannels) = decoderMatrix.row(idx1);
                        for (size_t n{0}; n < row.size(); ++n) {
                            DBG("Sending OSC: " << Params::getDecoderCoefficientParamID(1, n) <<
                                " " << juce::String{row[n]} << " to " << prop.name.toString());
                            bundle.addElement(juce::OSCMessage{Params::getDecoderCoefficientParamID(1, n), row[n]});
                        }

                        disconnect();
                        if (!connectToSocket(socket, prop.name.toString(), Sockets::DecoderMatrixCoefficientMessengerRemotePort)) {
                            std::cerr << "Failed to connect to socket using address " << prop.name.toString() <<
                                    ":" << Sockets::DecoderMatrixCoefficientMessengerRemotePort << std::endl;
                        }
                        send(bundle);

                        if (auto *moduleObj{module.getDynamicObject()}) {
                            moduleObj->setProperty(Utils::Identifiers::ModulePositionHasChangedPropertyID, false);
                            obj->setProperty(prop.name, module);
                        }
                    }
                }

                treeWhosePropertyHasChanged.setProperty(property, juce::var{obj}, nullptr);
            }
        }
    }

    template<int N>
    void DecoderMatrixCoefficientMessenger::sh_sn3d(float azimuth, const float elevation, std::array<float, (N + 1) * (N + 1)> &Y)
    {
        const float x = std::sin(elevation);
        const float s = std::cos(elevation); // sqrt(1 - x^2), always >= 0 for |el| <= pi/2

        // Associated Legendre P[l][m], m >= 0, without Condon-Shortley phase
        float P[N + 1][N + 1] = {};
        float pmm = 1.0f; // P_m^m = (2m-1)!! * s^m
        for (int m = 0; m <= N; ++m) {
            if (m > 0) pmm *= (2.f * m - 1.f) * s;
            P[m][m] = pmm;
            if (m + 1 <= N) P[m + 1][m] = (2.f * m + 1.f) * x * pmm;
            for (int l = m + 2; l <= N; ++l)
                P[l][m] = ((2.f * l - 1.f) * x * P[l - 1][m] - (l + m - 1) * P[l - 2][m]) / (l - m);
        }

        for (int l = 0; l <= N; ++l) {
            for (int m = -l; m <= l; ++m) {
                const int am = std::abs(m);
                float ratio = 1.0f; // (l-|m|)! / (l+|m|)!
                for (int k = l - am + 1; k <= l + am; ++k) ratio /= k;
                const float norm = std::sqrt((am == 0 ? 1.0f : 2.0f) * ratio);
                const float trig = (m >= 0) ? std::cos(am * azimuth) : std::sin(am * azimuth);
                Y[l * l + l + m] = norm * P[l][am] * trig; // ACN index = l^2 + l + m
            }
        }
    }

    DecoderMatrixCoefficientMessenger::Cartesian2Spherical::Cartesian2Spherical(float x, float y, float z)
    {
        const auto rtemp = std::sqrt(x * x + y * y + z * z);
        radius = rtemp == 0 ? 1 : rtemp;
        azimuth = std::atan2(y, x);
        elevation = std::asin(juce::jlimit(-1.0f, 1.0f, z / radius));
    }

    void DecoderMatrixCoefficientMessenger::computeDecoderMatrix()
    {
        const int K = Constants::NumChannels; // (order+1)^2
        const int L = sphericalHarmonics.size(); // number of speakers

        Eigen::MatrixXf Ys(K, L);
        for (int j = 0; j < L; ++j)
            Ys.col(j) = Eigen::Map<const Eigen::VectorXf>(sphericalHarmonics[j].data(), K);

        Eigen::JacobiSVD<Eigen::MatrixXf> svd(Ys, Eigen::ComputeThinU | Eigen::ComputeThinV);
        const auto &sv = svd.singularValues();

        juce::String svText;
        for (int i = 0; i < sv.size(); ++i)
            svText << sv(i) << " ";

        DBG("Singular values: " << svText);
        DBG("Condition number: " << (sv(0) / sv(sv.size() - 1)));

        // decoderMatrix = Ys.completeOrthogonalDecomposition().pseudoInverse(); // L x K

        // Regularized (damped) pseudo-inverse: tikhonov damping tames the
        // near-zero singular values from the array's elevation under-sampling
        const float lambda = 0.01f;   // tune: larger = more damping, smaller peak coefficients
        Eigen::VectorXf sInv = sv.array() / (sv.array().square() + lambda * lambda);
        decoderMatrix = svd.matrixV() * sInv.asDiagonal() * svd.matrixU().transpose(); // L x K
    }
}
