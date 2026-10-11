#include "ClientInfo.h"
#include <AnanasUtils.h>
#include "ServerUtils.h"

namespace ananas
{
    template<typename AnnouncePacketType>
    void ClientInfo<AnnouncePacketType>::update(const AnnouncePacketType *packet)
    {
        lastReceiveTime = juce::Time::getMillisecondCounter();
        info = *packet;
    }

    template<typename AnnouncePacketType>
    bool ClientInfo<AnnouncePacketType>::isConnected() const
    {
        return juce::Time::getMillisecondCounter() - lastReceiveTime < Server::Sockets::ClientListenerSocketParams.disconnectionThresholdMs;
    }

    template<typename AnnouncePacketType>
    AnnouncePacketType ClientInfo<AnnouncePacketType>::getInfo() const
    {
        return info;
    }

    template
    class ClientInfo<ClientAnnouncePacket>;

    template
    class ClientInfo<WfsClientAnnouncePacket>;

    template
    class ClientInfo<HoaClientAnnouncePacket>;

    //==========================================================================

    template<typename AnnouncePacketType>
    void ModuleInfo<AnnouncePacketType>::update()
    {
        lastReceiveTime = juce::Time::getMillisecondCounter();
    }

    template<typename AnnouncePacketType>
    juce::ValueTree ModuleInfo<AnnouncePacketType>::toValueTree() const
    {
        juce::ValueTree tree{"Module"};
        if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
            tree.setProperty(Utils::Identifiers::ModuleSecondarySource0xPropertyID, secondarySource0Position.first, nullptr);
            tree.setProperty(Utils::Identifiers::ModuleSecondarySource0yPropertyID, secondarySource0Position.second, nullptr);
            tree.setProperty(Utils::Identifiers::ModuleSecondarySource1xPropertyID, secondarySource1Position.first, nullptr);
            tree.setProperty(Utils::Identifiers::ModuleSecondarySource1yPropertyID, secondarySource1Position.second, nullptr);
        } else if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
            tree.setProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID, decoderMatrixRow0, nullptr);
            tree.setProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID, decoderMatrixRow1, nullptr);
        }
        return tree;
    }

    template<typename AnnouncePacketType>
    bool ModuleInfo<AnnouncePacketType>::isConnected() const
    {
        return juce::Time::getMillisecondCounter() - lastReceiveTime < Server::Sockets::ClientListenerSocketParams.disconnectionThresholdMs;
    }

    template<typename AnnouncePacketType>
    bool ModuleInfo<AnnouncePacketType>::justDisconnected()
    {
        const auto didJustDisconnect{!isConnected() && wasConnected};

        if (didJustDisconnect) {
            wasConnected = false;
        }

        return didJustDisconnect;
    }

    template<typename AnnouncePacketType>
    bool ModuleInfo<AnnouncePacketType>::justConnected()
    {
        const auto didJustConnect{isConnected() && !wasConnected};

        if (didJustConnect) {
            wasConnected = true;
        }

        return didJustConnect;
    }

    template<typename AnnouncePacketType>
    std::pair<float, float> ModuleInfo<AnnouncePacketType>::getSecondarySource0Position() const
    {
        if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
            return secondarySource0Position;
        } else {
            return std::make_pair(0.0f, 0.0f);
        }
    }

    template<typename AnnouncePacketType>
    std::pair<float, float> ModuleInfo<AnnouncePacketType>::getSecondarySource1Position() const
    {
        if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
            return secondarySource1Position;
        } else {
            return std::make_pair(0.0f, 0.0f);
        }
    }

    template <typename AnnouncePacketType>
    juce::int32 ModuleInfo<AnnouncePacketType>::getDecoderMatrixRow0() const
    {
        if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
            return decoderMatrixRow0;
        } else {
            return 0;
        }
    }

    template<typename AnnouncePacketType>
    juce::int32 ModuleInfo<AnnouncePacketType>::getDecoderMatrixRow1() const
    {
        if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
            return decoderMatrixRow1;
        } else {
            return 0;
        }
    }

    template<typename AnnouncePacketType>
    ModuleInfo<AnnouncePacketType> ModuleInfo<AnnouncePacketType>::fromValueTree(const juce::ValueTree &tree)
    {
        ModuleInfo info;
        if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
            const auto &prop0{tree.getProperty(Utils::Identifiers::ModuleSecondarySource0xPropertyID)};
            if (prop0.isDouble()) {
                const float val{prop0};
                info.secondarySource0Position.first = val;
            }
            const auto &prop1{tree.getProperty(Utils::Identifiers::ModuleSecondarySource0yPropertyID)};
            if (prop1.isDouble()) {
                const float val{prop1};
                info.secondarySource0Position.second = val;
            }
            const auto &prop2{tree.getProperty(Utils::Identifiers::ModuleSecondarySource0xPropertyID)};
            if (prop2.isDouble()) {
                const float val{prop2};
                info.secondarySource1Position.first = val;
            }
            const auto &prop3{tree.getProperty(Utils::Identifiers::ModuleSecondarySource0yPropertyID)};
            if (prop3.isDouble()) {
                const float val{prop3};
                info.secondarySource1Position.second = val;
            }
        } else if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
            const auto &prop0{tree.getProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID)};
            if (prop0.isInt()) {
                const int val{prop0};
                info.decoderMatrixRow0 = val;
            }
            const auto &prop1{tree.getProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID)};
            if (prop1.isInt()) {
                const int val{prop1};
                info.decoderMatrixRow1 = val;
            }
        }
        return info;
    }

    template
    class ModuleInfo<ClientAnnouncePacket>;

    template
    class ModuleInfo<WfsClientAnnouncePacket>;

    template
    class ModuleInfo<HoaClientAnnouncePacket>;

    //==========================================================================

    template<typename AnnouncePacketType>
    ClientList<AnnouncePacketType>::ClientList()
    {
        startTimer(Server::Constants::ClientConnectednessCheckIntervalMs);
    }

    template<typename AnnouncePacketType>
    void ClientList<AnnouncePacketType>::handlePacket(const juce::String &clientIP, const AnnouncePacketType *packet)
    {
        auto iter{clients.find(clientIP)};
        if (iter == clients.end()) {
            ClientInfo<AnnouncePacketType> c{};
            iter = clients.insert(std::make_pair(clientIP, c)).first;
            std::cout << "Client " << iter->first << " connected." << std::endl;
        }
        iter->second.update(packet);

        sendChangeMessage();
    }

    template<typename AnnouncePacketType>
    void ClientList<AnnouncePacketType>::timerCallback()
    {
        checkConnectivity();
    }

    template<typename AnnouncePacketType>
    juce::var ClientList<AnnouncePacketType>::toVar() const
    {
        const auto object{new juce::DynamicObject()};

        for (const auto &[ip, clientInfo]: clients) {
            auto *client{new juce::DynamicObject()};
            const auto &info{clientInfo.getInfo()};
            client->setProperty(Utils::Identifiers::ClientSerialNumberPropertyID, static_cast<int>(info.serial));
            client->setProperty(Utils::Identifiers::ClientFirmwareTypeVersionPropertyID,
                                Utils::FirmwareTypeToString(info.firmwareType) + " v" + Utils::VersionNumberToString(info.firmwareVersion));
            client->setProperty(Utils::Identifiers::ClientPTPLockPropertyID, info.ptpLock);
            client->setProperty(Utils::Identifiers::ClientPresentationTimeOffsetNsPropertyID, info.presentationOffsetNs);
            client->setProperty(Utils::Identifiers::ClientPresentationTimeOffsetFramePropertyID, info.presentationOffsetFrame);
            client->setProperty(Utils::Identifiers::ClientAudioPTPOffsetPropertyID, info.audioPTPOffsetNs);
            client->setProperty(Utils::Identifiers::ClientBufferFillPercentPropertyID, info.bufferFillPercent);
            client->setProperty(Utils::Identifiers::ClientSamplingRatePropertyID, info.samplingRate);
            client->setProperty(Utils::Identifiers::ClientPercentCPUPropertyID, info.percentCPU);
            if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
                client->setProperty(Utils::Identifiers::ClientSecondarySourceCoordinatesPropertyID,
                                    "(" + juce::String{info.secondarySource0x} + ", " + juce::String{info.secondarySource0y} +
                                    "), (" + juce::String{info.secondarySource1x} + ", " + juce::String{info.secondarySource1y} + ")");
            } else if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
                client->setProperty(Utils::Identifiers::ClientDecoderMatrixRowsPropertyID, juce::String{info.decoderMatrixRow0} + ", " +
                                                                                           juce::String{info.decoderMatrixRow1});
            }
            object->setProperty(ip, client);
        }

        return object;
    }

    template<typename AnnouncePacketType>
    bool ClientList<AnnouncePacketType>::getShouldReboot() const
    {
        return shouldReboot;
    }

    template<typename AnnouncePacketType>
    void ClientList<AnnouncePacketType>::setShouldReboot(const bool should)
    {
        shouldReboot = should;
    }

    template<typename AnnouncePacketType>
    uint ClientList<AnnouncePacketType>::getCount() const
    {
        return clients.size();
    }

    template<typename AnnouncePacketType>
    juce::ValueTree ClientList<AnnouncePacketType>::toValueTree()
    {
        juce::ValueTree tree(Utils::Identifiers::ConnectedClientsParamID);

        for (const auto &[ip, _]: clients) {
            juce::ValueTree subTree("Client");
            subTree.setProperty("ip", ip, nullptr);
            tree.addChild(subTree, -1, nullptr);
        }

        return tree;
    }

    template<typename AnnouncePacketType>
    void ClientList<AnnouncePacketType>::checkConnectivity()
    {
        std::vector<juce::String> toErase;
        for (const auto &[ip, c]: clients) {
            if (!c.isConnected()) {
                std::cout << "Client " << ip << " disconnected." << std::endl;
                toErase.push_back(ip);
            }
        }
        for (const auto &ip: toErase) {
            clients.erase(ip);
            sendChangeMessage();
        }
    }

    template
    class ClientList<ClientAnnouncePacket>;

    template
    class ClientList<WfsClientAnnouncePacket>;

    template
    class ClientList<HoaClientAnnouncePacket>;

    //==========================================================================

    template <typename AnnouncePacketType>
    ModuleList<AnnouncePacketType>::ModuleList()
    {
        startTimer(Server::Constants::ClientConnectednessCheckIntervalMs);
    }

    template <typename AnnouncePacketType>
    void ModuleList<AnnouncePacketType>::handlePacket(const juce::String &moduleIP)
    {
        auto iter{modules.find(moduleIP)};
        if (iter == modules.end()) {
            ModuleInfo<AnnouncePacketType> m{};
            iter = modules.insert(std::make_pair(moduleIP, m)).first;
            std::cout << "Module " << moduleIP << " available." << std::endl;
        }
        iter->second.update();
        if (iter->second.justConnected()) {
            std::cout << "Module " << iter->first << " just connected." << std::endl;
            sendChangeMessage();
        }
    }

    template <typename AnnouncePacketType>
    void ModuleList<AnnouncePacketType>::timerCallback()
    {
        checkConnectivity();
    }

    template <typename AnnouncePacketType>
    juce::var ModuleList<AnnouncePacketType>::toVar() const
    {
        const auto object{new juce::DynamicObject()};

        for (const auto &[ip, m]: modules) {
            auto *module{new juce::DynamicObject()};
            if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
                module->setProperty(Utils::Identifiers::ModuleSecondarySource0xPropertyID, static_cast<int>(m.getSecondarySource0Position().first));
                module->setProperty(Utils::Identifiers::ModuleSecondarySource0yPropertyID, static_cast<int>(m.getSecondarySource0Position().second));
                module->setProperty(Utils::Identifiers::ModuleSecondarySource1xPropertyID, static_cast<int>(m.getSecondarySource1Position().first));
                module->setProperty(Utils::Identifiers::ModuleSecondarySource1yPropertyID, static_cast<int>(m.getSecondarySource1Position().second));
            } else if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
                module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow0PropertyID, static_cast<int>(m.getDecoderMatrixRow0()));
                module->setProperty(Utils::Identifiers::ModuleDecoderMatrixRow1PropertyID, static_cast<int>(m.getDecoderMatrixRow1()));
            }
            module->setProperty(Utils::Identifiers::ModuleIsConnectedPropertyID, m.isConnected());
            object->setProperty(ip, module);
        }

        return object;
    }

    template <typename AnnouncePacketType>
    juce::ValueTree ModuleList<AnnouncePacketType>::toValueTree() const
    {
        juce::ValueTree tree(Utils::Identifiers::ModulesParamID);

        for (const auto &[ip, m]: modules) {
            auto moduleTree{m.toValueTree()};
            moduleTree.setProperty("ip", ip, nullptr);
            tree.addChild(moduleTree, -1, nullptr);
        }

        return tree;
    }

    template <typename AnnouncePacketType>
    void ModuleList<AnnouncePacketType>::fromValueTree(const juce::ValueTree &tree)
    {
        modules.clear();

        for (int i{0}; i < tree.getNumChildren(); ++i) {
            auto moduleTree{tree.getChild(i)};
            juce::Identifier ip{moduleTree.getProperty("ip")};
            modules[ip.toString()] = ModuleInfo<AnnouncePacketType>::fromValueTree(moduleTree);
        }
    }

    template <typename AnnouncePacketType>
    void ModuleList<AnnouncePacketType>::checkConnectivity()
    {
        for (auto it{modules.begin()}; it != modules.end(); ++it) {
            if (it->second.justDisconnected()) {
                std::cout << "Module " << it->first << " just disconnected." << std::endl;
                sendChangeMessage();
            }
        }
    }

    template
    class ModuleList<ClientAnnouncePacket>;

    template
    class ModuleList<WfsClientAnnouncePacket>;

    template
    class ModuleList<HoaClientAnnouncePacket>;
}
