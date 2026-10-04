#ifndef DECODERMATRIXCOEFFICIENTMESSENGER_H
#define DECODERMATRIXCOEFFICIENTMESSENGER_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_osc/juce_osc.h>
#include "HOAUtils.h"
#include <Eigen/Dense>

namespace ananas::HOA
{
    class DecoderMatrixCoefficientMessenger final : public juce::ValueTree::Listener,
                                                    public juce::OSCSender
    {
    public:
        explicit DecoderMatrixCoefficientMessenger(juce::AudioProcessorValueTreeState &apvts);


        void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;

    private:
        struct Cartesian2Spherical
        {
            Cartesian2Spherical(float x, float y, float z);

            float azimuth, elevation, radius;
        };

        // N = ambisonic order, output has (N+1)^2 gains in ACN order.
        // az: counter-clockwise from front, el: up from horizontal, both in radians.
        template<int N>
        void sh_sn3d(float azimuth, float elevation, std::array<float, (N + 1) * (N + 1)> &Y);

        void computeDecoderMatrix();

        juce::DatagramSocket socket;
        juce::AudioProcessorValueTreeState &state;
        juce::Array<Cartesian2Spherical> speakerPositions;
        juce::Array<std::array<float, Constants::NumChannels> > sphericalHarmonics;
        Eigen::MatrixXf decoderMatrix;
    };
}


#endif //DECODERMATRIXCOEFFICIENTMESSENGER_H
