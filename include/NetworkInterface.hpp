#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <queue>
#include "NetworkEvents.h"
class Game;
/**< Handles network operatations that the game requires it to do */
class NetworkInterface : sf::NonCopyable
{
public:
    NetworkInterface();
    virtual ~NetworkInterface();
	/**< Connect tcp socket to specified ip */
    bool connectToIp(const sf::IpAddress & ip);
    /**< Send the server that we are connected to a message that we are about to disconnect */
    void disconnectFromServer();
    /**< Calls hangleMessage in the game class for each packet that has to be processed. */
    void handleMessages(Game & game);
	/**< Ask the master server to send a server list to this client */
    void askForServerList();
    /**< Send state of player over to the server */
    void sendStateUpdate();

    bool getConnected()const{return isConnected_;}
    void sendAuthorizeMessage();


protected:
private:

    sf::Thread tcpReceiveThread_;
    sf::Thread udpReceiveThread_;
    /**< Launches tcp receive thread and udp receive thread */
    void launch();
    /**< Listens for incoming tcp messages on the connection to the server */
    void tcpListen();
    /**< Listens for incoming udp messages on the client port, can be from anyone, should be from server */
    void udpListen();

    sf::UdpSocket udpSocket_;
    sf::TcpSocket tcpSocket_;

    bool isRunning_;
    bool isConnected_;
    bool isUdpBound_;

    bool getRunning()const
    {
        return isRunning_;
    }

    //queue where messages are stored before they are read
    void receiveMessage(const sf::Packet & pack, const sf::IpAddress & ip, unsigned short port);
    std::queue<PacketWrap> toRead_;

private:
    //handles
    void onConnectionSuccess();

};

#endif // NETWORKINTERFACE_H
