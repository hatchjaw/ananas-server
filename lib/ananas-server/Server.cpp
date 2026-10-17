#include <arpa/inet.h>
#include "Server.h"
#include <AnanasUtils.h>
#include <AuthorityInfo.h>

#include "../../src/wfs-plugin/UI/WFSInterfaceComponent.h"

namespace ananas::Server
{
    ServerBase::ServerBase(const uint numChannelsToSend) : numChannels(numChannelsToSend),
                                                           fifo(numChannelsToSend)
    {
    }

    ServerBase::~ServerBase()
    {
        ServerBase::releaseResources();

        for (const auto &t: threads) {
            t->removeChangeListener(this);
        }
    }

    void ServerBase::prepareToPlay(const int samplesPerBlockExpected, const double sampleRate)
    {
        for (const auto &t: threads) {
            if (auto *s = dynamic_cast<AudioSender *>(t)) {
                // The audio sender needs to be prepared; other threads do not.
                s->prepare(numChannels, samplesPerBlockExpected, sampleRate);
            }
            // With the audio sender thread prepared, and memory allocated to
            // its AudioPacket member, it's safe to start all the threads.
            t->startThread();
        }
    }

    void ServerBase::releaseResources()
    {
        for (const auto &t: threads) {
            if (t->isThreadRunning()) {
                if (auto *s = dynamic_cast<AudioSender *>(t)) {
                    // The sender thread overrides stopThread, so handle it
                    // separately.
                    s->stopThread(t->getTimeout());
                } else {
                    // Stop all the other threads as normal.
                    t->stopThread(t->getTimeout());
                }
            }
        }
    }

    void ServerBase::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
    {
        // These checks are kind of overkill, since the order in which threads
        // were added to the OwnedArray is known, but as a sanity check...
        if (auto *s = dynamic_cast<AudioSender *>(threads[0])) {
            if (auto *t = dynamic_cast<TimestampListener *>(threads[1])) {
                // If a new timestamp is available, send it to the audio sender
                // to see whether the packet timestamp needs to be updated.
                if (t->isNewTimestampAvailable()) {
                    s->setPacketTime(t->getTimestamp());
                }
            }
        }

        fifo.write(bufferToFill.buffer);
    }

    void ServerBase::changeListenerCallback(ChangeBroadcaster *source)
    {
        // If one of the threads announces a change, broadcast that change to
        // any listeners.
        sendChangeMessage();
    }

    bool ServerBase::isConnected() const
    {
        return std::all_of(threads.begin(), threads.end(), [](const AnanasThread *t)
        {
            return t->isConnected();
        });
    }

    AuthorityInfo *ServerBase::getAuthority()
    {
        return &authority;
    }

    SwitchList *ServerBase::getSwitches()
    {
        return &switches;
    }


    //==========================================================================

    ServerBase::AnanasThread::AnanasThread(const Utils::ThreadParams &p)
        : Thread(p.name), timeoutMs(p.timeoutMs)
    {
    }

    void ServerBase::AnanasThread::run()
    {
        while (!connect() && !threadShouldExit()) {
            for (uint i{0}; i < Constants::ThreadConnectWaitIterations && !threadShouldExit(); ++i)
                wait(Constants::ThreadConnectWaitIntervalMs);
        }

        if (!threadShouldExit()) {
            connected = true;
            runImpl();
        }
    }

    int ServerBase::AnanasThread::getTimeout() const
    {
        return timeoutMs;
    }

    bool ServerBase::AnanasThread::isConnected() const
    {
        return connected;
    }

    //==========================================================================

    ServerBase::UDPMulticastThread::UDPMulticastThread(const Utils::ThreadSocketParams &p)
        : AnanasThread(p),
          ip(p.ip),
          localPort(p.localPort)
    {
    }

    ServerBase::UDPMulticastThread::~UDPMulticastThread()
    {
        socket.leaveMulticast(ip);
        socket.shutdown();
    }

    bool ServerBase::UDPMulticastThread::connect()
    {
        if (-1 == socket.getBoundPort()) {
            if (!socket.setEnablePortReuse(true)) {
                std::cerr << getThreadName() << " failed to set socket port reuse: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            if (!socket.bindToPort(localPort, Utils::Strings::LocalInterfaceIP)) {
                std::cerr << getThreadName() << " failed to bind socket to port: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            if (!socket.joinMulticast(ip)) {
                std::cerr << getThreadName() << " failed to join multicast group: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            socket.setMulticastLoopbackEnabled(false);
            socket.waitUntilReady(false, 1000);
        }

        sendChangeMessage();
        return true;
    }

    //==========================================================================

    ServerBase::SenderThread::SenderThread(const Utils::SenderThreadSocketParams &p)
        : UDPMulticastThread(p),
          remotePort(p.remotePort)
    {
    }

    //==========================================================================

    ServerBase::AudioSender::AudioSender(const Utils::SenderThreadSocketParams &p, Fifo &fifo)
        : SenderThread(p),
          fifo(fifo)
    {
    }

    bool ServerBase::AudioSender::prepare(const uint numChannels, const int samplesPerBlockExpected, const double sampleRate)
    {
        juce::ignoreUnused(sampleRate);

        audioBlockSamples = samplesPerBlockExpected;

        packet.prepare(numChannels, Constants::FramesPerPacket, sampleRate);

        return startThread();
    }

    void ServerBase::AudioSender::setPacketTime(const timespec ts)
    {
        packet.setTime(ts);
    }

    int64_t ServerBase::AudioSender::getPacketTime() const
    {
        return packet.getTime();
    }

    bool ServerBase::AudioSender::stopThread(const int timeOutMilliseconds)
    {
        fifo.abortRead();
        return Thread::stopThread(timeOutMilliseconds);
    }

    void ServerBase::AudioSender::runImpl()
    {
        std::cout << getThreadName() << " sending audio packets..." << std::endl << std::flush;

        while (!threadShouldExit()) {
            // Read from the fifo into the packet.
            fifo.read(packet.getAudioData(), Constants::FramesPerPacket);
            if (threadShouldExit()) break;
            // Write the header to the packet.
            packet.writeHeader();
            // Write the packet to the socket.
            socket.write(ip, remotePort, packet.getData(), static_cast<int>(packet.getSize()));

            const timespec t{0, packet.getSleepInterval()};
            nanosleep(&t, nullptr);
        }

        std::cout << getThreadName() << " stopping." << std::endl;
    }

    //==========================================================================

    ServerBase::AnnouncementListenerThread::AnnouncementListenerThread(
        const Utils::ListenerThreadSocketParams &p
    ) : UDPMulticastThread(p)
    {
    }

    bool ServerBase::AnnouncementListenerThread::connect()
    {
        if (-1 == socket.getBoundPort()) {
            // JUCE doesn't handle multicast in a manner that's compatible with
            // reading multicast packets on a specific interface...

            // ...(it's probably not necessary to allow port-reuse, but what the
            // hell)...
            if (!socket.setEnablePortReuse(true)) {
                std::cerr << getThreadName() << " failed to set socket port reuse: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            // ...bind the relevant port to ALL interfaces (INADDR_ANY) by not
            // specifying a local interface here...
            if (!socket.bindToPort(localPort)) {
                std::cerr << getThreadName() << " failed to bind socket to port: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            // ...then join the appropriate multicast group on the relevant
            // interface (i.e. a manually-configured ethernet connection).
            ip_mreq mreq{};
            mreq.imr_multiaddr.s_addr = inet_addr(ip.toRawUTF8());
            mreq.imr_interface.s_addr = inet_addr(Utils::Strings::LocalInterfaceIP.text);

            if (setsockopt(
                    socket.getRawSocketHandle(),
                    IPPROTO_IP,
                    IP_ADD_MEMBERSHIP,
                    &mreq, sizeof (mreq)) < 0) {
                std::cerr << getThreadName() << " failed to add multicast membership: " << strerror(errno) << std::endl;
                sendChangeMessage();
                return false;
            }

            socket.setMulticastLoopbackEnabled(false);
        }

        sendChangeMessage();
        return true;
    }

    void ServerBase::AnnouncementListenerThread::runImpl()
    {
        std::cout << getThreadName() << " listening..." << std::endl << std::flush;

        while (!threadShouldExit()) {
            if (socket.waitUntilReady(true, timeoutMs)) {
                if (threadShouldExit()) break;

                if (const auto bytesRead{
                    socket.read(buffer, Constants::ListenerBufferSize, false, senderIP, senderPort)
                }; bytesRead > 0) {
                    handlePacket();
                } else if (bytesRead < 0) {
                    std::cerr << getThreadName() << ": error reading from socket: " << strerror(errno) << std::endl;
                }
            }
        }

        std::cout << getThreadName() << " stopping." << std::endl;
    }

    //==============================================================================

    ServerBase::TimestampListener::TimestampListener(
        const Utils::ListenerThreadSocketParams &p
    ) : AnnouncementListenerThread(p)
    {
    }

    bool ServerBase::TimestampListener::isNewTimestampAvailable()
    {
        return newTimestampAvailable.exchange(false, std::memory_order_acquire);
    }

    timespec ServerBase::TimestampListener::getTimestamp() const noexcept
    {
        return timestamp.load(std::memory_order_acquire);
    }

    void ServerBase::TimestampListener::handlePacket()
    {
        // Check for Follow_Up message (0x08)
        if ((buffer[0] & 0x0f) == Constants::PTPFollowUpMessageType) {
            timespec ts{};

            // Extract seconds (6 bytes)
            for (int i = 0; i < 6; i++) {
                ts.tv_sec = ts.tv_sec << 8 | buffer[34 + i];
            }

            // Extract nanoseconds (4 bytes)
            for (int i = 0; i < 4; i++) {
                ts.tv_nsec = ts.tv_nsec << 8 | buffer[40 + i];
            }

            // Store the new timestamp and indicate that it is available.
            timestamp.store(ts, std::memory_order_release);
            newTimestampAvailable.store(true, std::memory_order_release);
        }
    }

    //==============================================================================

    template<typename AnnouncePacketType>
    ServerBase::ClientListener<AnnouncePacketType>::ClientListener(
        const Utils::ListenerThreadSocketParams &p,
        ClientList<AnnouncePacketType> &clients,
        ModuleList<AnnouncePacketType> &modules
    ) : AnnouncementListenerThread(p),
        clients(clients),
        modules(modules)
    {
    }

    template<typename AnnouncePacketType>
    void ServerBase::ClientListener<AnnouncePacketType>::handlePacket()
    {
        clients.handlePacket(senderIP, reinterpret_cast<const AnnouncePacketType *>(buffer));
        modules.handlePacket(senderIP);
    }

    //==============================================================================

    ServerBase::AuthorityListener::AuthorityListener(
        const Utils::ListenerThreadSocketParams &p,
        AuthorityInfo &authority
    ) : AnnouncementListenerThread(p),
        authority(authority)
    {
    }

    void ServerBase::AuthorityListener::handlePacket()
    {
        authority.handlePacket(senderIP, reinterpret_cast<AuthorityAnnouncePacket *>(buffer));
    }

    //==========================================================================

    template <typename AnnouncePacketType>
    ServerBase::RebootSender<AnnouncePacketType>::RebootSender(
        const Utils::SenderThreadSocketParams &p,
        ClientList<AnnouncePacketType> &clients
    ): SenderThread(p),
       clients(clients)
    {
    }

    template <typename AnnouncePacketType>
    void ServerBase::RebootSender<AnnouncePacketType>::runImpl()
    {
        while (!threadShouldExit()) {
            if (clients.getShouldReboot()) {
                clients.setShouldReboot(false);
                socket.write(ip, remotePort, nullptr, 0);
            }

            // Wait 1 second, but check for thread exit every 100ms
            for (int i = 0; i < 10 && !threadShouldExit(); ++i)
                wait(100);
        }
    }

    template
    class ServerBase::RebootSender<ClientAnnouncePacket>;

    template
    class ServerBase::RebootSender<WfsClientAnnouncePacket>;

    template
    class ServerBase::RebootSender<HoaClientAnnouncePacket>;

    //==========================================================================

    ServerBase::SwitchInspector::SwitchInspector(
        const Utils::ThreadParams &p,
        SwitchList &switches
    ) : AnanasThread(p),
        switches(switches)
    {
    }

    void ServerBase::SwitchInspector::runImpl()
    {
        std::cout << getThreadName() << " inspecting..." << std::endl << std::flush;

        while (!threadShouldExit()) {
            auto switchesVar{switches.toVar()};

            if (auto *obj = switchesVar.getDynamicObject()) {
                for (const auto &prop: obj->getProperties()) {
                    if (const auto *s = prop.value.getDynamicObject()) {
                        auto ip{s->getProperty(Utils::Identifiers::SwitchIpPropertyID).toString()};
                        auto username{s->getProperty(Utils::Identifiers::SwitchUsernamePropertyID).toString()};
                        auto password{s->getProperty(Utils::Identifiers::SwitchPasswordPropertyID).toString()};
                        const bool shouldResetPtp{s->getProperty(Utils::Identifiers::SwitchShouldResetPtpPropertyID)};

                        if (ip.isEmpty() || username.isEmpty() || password.isEmpty()) break;

                        juce::var jsonData{new juce::DynamicObject};
                        juce::var response;

                        if (shouldResetPtp) {
                            jsonData.getDynamicObject()->setProperty("numbers", "0");
                            response = curlRequest(ip, username, password, Constants::SwitchDisablePtpPath, juce::JSON::toString(jsonData));
                            switches.handleResponse(prop.name, response);
                            response = curlRequest(ip, username, password, Constants::SwitchEnablePtpPath, juce::JSON::toString(jsonData));
                            switches.handleResponse(prop.name, response);
                        } else {
                            jsonData.getDynamicObject()->setProperty("numbers", "0");
                            jsonData.getDynamicObject()->setProperty("once", "");
                            response = curlRequest(ip, username, password, Constants::SwitchMonitorPtpPath, juce::JSON::toString(jsonData));
                            switches.handleResponse(prop.name, response);
                        }
                    }
                }
            }

            // Wait 1 second, but check for thread exit every 100ms
            for (int i = 0; i < 10 && !threadShouldExit(); ++i)
                wait(100);
        }

        std::cout << getThreadName() << " stopping." << std::endl;
    }

    bool ServerBase::SwitchInspector::connect()
    {
        sendChangeMessage();
        return true;
    }

    juce::var ServerBase::SwitchInspector::curlRequest(const juce::String &ip,
                                                       const juce::String &username,
                                                       const juce::String &password,
                                                       const juce::StringRef &path,
                                                       const juce::String &postData)
    {
        const juce::URL url("http://" + ip + path);

        juce::StringArray args;
        args.add("curl");
        args.add("-s"); // Silent, no stats
        args.add("-m" + juce::String{Constants::SwitchInspectorRequestTimeoutS});
        args.add("-k"); // Insecure (no TLS)
        args.add("-u"); // Specify username and password
        args.add(username + ":" + password);
        args.add(url.toString(false));
        args.add("-H");
        args.add("Content-Type: application/json");
        args.add("-d");
        args.add(postData);

        if (curl.start(args)) {
            const auto response{curl.readAllProcessOutput()};
            return juce::JSON::parse(response);
        }
        return juce::var{};
    }

    //==========================================================================

    template<typename AnnouncePacketType>
    Server<AnnouncePacketType>::Server(const uint numChannelsToSend)
        : ServerBase(numChannelsToSend)
    {
        // Add all the threads.
        threads.add(new AudioSender(Sockets::AudioSenderSocketParams, fifo));
        threads.add(new TimestampListener(Sockets::TimestampListenerSocketParams));
        threads.add(new ClientListener<AnnouncePacketType>(Sockets::ClientListenerSocketParams, clients, modules));
        threads.add(new AuthorityListener(Sockets::AuthorityListenerSocketParams, authority));
        threads.add(new RebootSender<AnnouncePacketType>(Sockets::RebootSenderSocketParams, clients));
        threads.add(new SwitchInspector(Threads::SwitchInspectorThreadParams, switches));

        // The server should listen for change messages sent by all threads.
        for (const auto &t: threads) {
            t->addChangeListener(this);
        }
    }

    template<typename AnnouncePacketType>
    ClientList<AnnouncePacketType> *Server<AnnouncePacketType>::getClientList()
    {
        return &clients;
    }

    template<typename AnnouncePacketType>
    ModuleList<AnnouncePacketType> *Server<AnnouncePacketType>::getModuleList()
    {
        return &modules;
    }

    template
    class Server<ClientAnnouncePacket>;

    template
    class Server<WfsClientAnnouncePacket>;

    template
    class Server<HoaClientAnnouncePacket>;
}
