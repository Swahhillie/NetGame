#include "NetworkInterface.hpp"
#include "Global_Constants.hpp"
#include "FullExpressionAccumulator.hpp"
#include "Mutexes.hpp"
#include "Game.hpp"
#include "DebugSpace.h"
#include "LocalPlayerInfo.hpp"
#include "ConfigLoader.hpp"


NetworkInterface::NetworkInterface():
    tcpReceiveThread_(&NetworkInterface::tcpListen, this),
    udpReceiveThread_(&NetworkInterface::udpListen, this)

{
    //ctor
    isRunning_ = true;
    isConnected_ = false;
    isUdpBound_ = false;

    launch();

}

NetworkInterface::~NetworkInterface()
{
    //dtor
    isRunning_ = false;
    udpSocket_.setBlocking(false);
    tcpSocket_.setBlocking(false);
    //udpReceiveThread_.wait(); //block until the receivethread finishes
    //tcpReceiveThread_.wait(); //block until the receivethread finishes

}
void NetworkInterface::launch()
{
    udpReceiveThread_.launch();
    tcpReceiveThread_.launch();
}
void NetworkInterface::udpListen()
{
    FullExpressionAccumulator(std::cout) << "NetworkInterface::udpListen() Started\n";
    if(Network::bindUdpAndReport(udpSocket_, sf::Socket::AnyPort))
    {
        isUdpBound_ = true;
        while(getRunning())
        {
            //received a udp message
            sf::Packet receivedPacket;
            sf::IpAddress remoteIp;
            unsigned short remotePort;
            if(udpSocket_.receive(receivedPacket, remoteIp, remotePort) == sf::Socket::Done)
            {
                //receiving successfull, adding to inbox
                receiveMessage(receivedPacket, remoteIp, remotePort);
                //if(Debug::VERBOSE_MESSAGES)
                  //  FullExpressionAccumulator(std::cout) << "A udp message was received\n";
            }
            else
            {
                FullExpressionAccumulator(std::cout) << "Failed to receive upd packet\n";
            }

        }
    }
    FullExpressionAccumulator(std::cout) << "NetworkInterface::udpListen() Ended\n";


}
void NetworkInterface::tcpListen()
{
    FullExpressionAccumulator(std::cout) << "NetworkInterface::tcpListen() Started\n";
    while(getRunning())
    {
        if(isConnected_)
        {
            sf::Packet receivedPacket;
            sf::Socket::Status status = tcpSocket_.receive(receivedPacket);

            if(status == sf::Socket::Done)
            {
                //receiving successfull, adding to inbox
                receiveMessage(receivedPacket, tcpSocket_.getRemoteAddress(), tcpSocket_.getRemotePort());
                if(Debug::VERBOSE_MESSAGES)
                    FullExpressionAccumulator(std::cout) << "TCP message received from server\n";
            }
            else if (status == sf::Socket::Disconnected)
            {
                //server has been disconnected
                isConnected_ = false;
                //attempt to reconnect later
                FullExpressionAccumulator(std::cout) << "TCP connection with server was disconnected\n";
            }
            else
            {
                FullExpressionAccumulator(std::cout) << "Receiving tcp packet from " << tcpSocket_.getRemoteAddress().toString() << " failed with status " << status << "\n";
            }
        }
        else
        {
        	//stop this thread from taking 100% cpu time while not connected
            //if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Tcp is not connected, going to sleeping\n";
            sf::sleep(sf::seconds(1.0f));
            //if(Debug::VERBOSE_MESSAGES) FullExpressionAccumulator(std::cout) <<"Tcp listen woke up\n";

        }
    }
    FullExpressionAccumulator(std::cout) << "NetworkInterface::tcpListen() Ended\n";
}
/*
void NetworkInterface::listen()
{
    FullExpressionAccumulator(std::cout) << "NetworkInterface::listen() STARTED\n";
    selector_.add(udpSocket_);
    while(getRunning())
    {
        if(selector_.wait(sf::seconds(.4f)))
        {
            if(isConnected_)
            {
                if(selector_.isReady(tcpSocket_))
                {

                    sf::Packet receivedPacket;
                    sf::Socket::Status status = tcpSocket_.receive(receivedPacket);

                    if(status == sf::Socket::Done)
                    {
                        //receiving successfull, adding to inbox
                        receiveMessage(receivedPacket);
                        if(Debug::VERBOSE_MESSAGES)
                            FullExpressionAccumulator(std::cout) << "TCP message received from server\n";
                    }
                    else if (status == sf::Socket::Disconnected)
                    {
                        //server has been disconnected
                        selector_.remove(tcpSocket_);
                        isConnected_ = false;
                        //attempt to reconnect later
                        FullExpressionAccumulator(std::cout) << "TCP connection with server was disconnected\n";
                    }
                    else
                    {
                        FullExpressionAccumulator(std::cout) << "Receiving tcp packet from " << tcpSocket_.getRemoteAddress().toString() << " failed with status " << status << "\n";
                    }
                }
            }
        }
        if(isUdpBound_)
        {
            if(selector_.isReady(udpSocket_))
            {
                //received a udp message
                sf::Packet receivedPacket;
                sf::IpAddress remoteIp;
                unsigned short remotePort;
                if(udpSocket_.receive(receivedPacket, remoteIp, remotePort) == sf::Socket::Done)
                {
                    //receiving successfull, adding to inbox
                    receiveMessage(receivedPacket);
                    if(Debug::VERBOSE_MESSAGES)
                        FullExpressionAccumulator(std::cout) << "A udp message was received\n";
                }
                else
                {
                    FullExpressionAccumulator(std::cout) << "Failed to receive upd packet\n";
                }

            }
        }


    }
    FullExpressionAccumulator(std::cout) <<"NetworkInterface::listen() ENDED\n";

}
*/
void NetworkInterface::receiveMessage(const sf::Packet & pack, const sf::IpAddress & ip, unsigned short port)
{
//    if(Debug::VERBOSE_MESSAGES)
//        FullExpressionAccumulator(std::cout) << "received a message\n";

    Mutexes::messages.lock();
    toRead_.push(PacketWrap(pack, ip , port)); // copy into queue, to be read by main thread later
    Mutexes::messages.unlock();
}
bool NetworkInterface::connectToIp(const sf::IpAddress & ip)
{
    if(tcpSocket_.connect(ip, SERVERPORT, sf::seconds(5.0f) )== sf::Socket::Done)
    {
        FullExpressionAccumulator(std::cout) << " Successfully connected to " << ip.toString() << " : " << "\n";
        onConnectionSuccess();
        isConnected_ = true;
        return true;
    }
    else
    {
        FullExpressionAccumulator(std::cout) << " Failed to connected to " << ip.toString() << " : " << SERVERPORT << "\n";
        return false;
    }
}
void NetworkInterface::disconnectFromServer()
{
    if(isConnected_)
    {
        FullExpressionAccumulator(std::cout) << "Sending a disconnection event to the server \n";
        sf::Packet disconnectEventPacket;
        PlayerDisconnected disconnectEvent;
        disconnectEvent.name = LocalPlayerInfo::instance().name;
        disconnectEventPacket << disconnectEvent;


        if(Network::sendTcpAndReport(tcpSocket_, disconnectEventPacket))
        {

        }
        else
        {
            FullExpressionAccumulator(std::cout) << "Failed to send tcp message";
        }

    }
    else
    {
        FullExpressionAccumulator(std::cout) << "Disconnecting from server but server is not connected, no need to disconnect\n";
    }

}
void NetworkInterface::onConnectionSuccess()
{
   FullExpressionAccumulator(std::cout) << "Established a connection to server " << tcpSocket_.getRemoteAddress().toString() << " : " << tcpSocket_.getRemotePort() << "\n";
}
void NetworkInterface::sendAuthorizeMessage(){
	FullExpressionAccumulator(std::cout) << "Sending authorization message to server \n";
	sf::Packet packet;
    PlayerAuthorize playerAuthorize;
    playerAuthorize.name = LocalPlayerInfo::instance().name;
    playerAuthorize.password = LocalPlayerInfo::instance().password;
    playerAuthorize.port = udpSocket_.getLocalPort();
    playerAuthorize.ip = sf::IpAddress::None;
    playerAuthorize.isRegistration = LocalPlayerInfo::instance().isRegistration;

    packet << playerAuthorize;
    if(Network::sendTcpAndReport(tcpSocket_, packet))
    {

    }
    else
    {
		assert(1 == 2);
    }
}
void NetworkInterface::handleMessages(Game & game)
{
    while(!toRead_.empty())
    {
        //if(Debug::VERBOSE_MESSAGES)
          //  FullExpressionAccumulator(std::cout) << "Getting message from the inbox to be handled\n";

        Mutexes::messages.lock();
        PacketWrap packetWrap = toRead_.front();
        toRead_.pop();
        Mutexes::messages.unlock();
        game.handleMessage(packetWrap);
    }
}
void NetworkInterface::askForServerList()
{
    sf::Packet p;
    ServerEntryGet serverEntryGet;
    p << serverEntryGet;
    Network::sendUdpAndReport(udpSocket_, p, ConfigLoader::instance().getMasterServerAddress(), MASTERSERVERPORT);
}
void NetworkInterface::sendStateUpdate()
{
    if(isConnected_)
    {
        LocalPlayerInfo & playerInfo = LocalPlayerInfo::instance();
        if(playerInfo.shouldSendNewKeyState)
        {
            sf::Packet playerInfoPacket;
            PlayerStateUpdate playerStateUpdate;
            playerStateUpdate << playerInfo;
            playerInfoPacket << playerStateUpdate;
            Network::sendTcpAndReport(tcpSocket_, playerInfoPacket);
            playerInfo.shouldSendNewKeyState = false;
        }
    }
}
