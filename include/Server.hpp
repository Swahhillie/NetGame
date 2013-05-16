#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <list>
#include <map>
#include <queue>
#include "NetworkEvents.h"
#include "ClientsManager.hpp"
#include "ServerCommandHandler.hpp"
#include "DatabaseManager.hpp"
#include "Level.hpp"

class Server
{
	friend class ServerCommandHandler;
public:
    Server();
    void build();
    void run();
    virtual ~Server();



protected:
private:
	//handeling clients
    ClientsManager clientsManager_;
    ServerCommandHandler serverCommandHandler_;
    DatabaseManager databaseManager_;

    sf::Thread tcpReceiver; //thread for listening to the socket selector
    sf::Thread udpReceiver;
    void listenSocketSelector();
    void listenUdp();
    float updatesPerSecond;
    void sendStateUpdates();

private:

    //!handeling of messages
    typedef std::queue<PacketWrap> MessageQueue;

    //containers for the messages
    MessageQueue toRead;
    MessageQueue toSend; // currently unused

    //BOTH
    void addMessageToRead(const sf::Packet & packet, const sf::IpAddress & ip, unsigned short port);
    sf::SocketSelector selector_;

    //TCP
    sf::TcpListener listener_;
    typedef std::list<sf::TcpSocket*> TcpConnections;
    TcpConnections tcpConnections;
    void acceptNewTcpConnection(sf::TcpListener & listener);
    void readMessagesFromTcpSockets();

    //UDP
    sf::UdpSocket udpSocket_;
    void readMessageFromUdpSocket(sf::UdpSocket & sock);

    Level lvl_;
    void serverLoadLevel(const std::string & name);

private:
    //! Database

private:
    //! Other
    void requestServerEntryAdd();
    //bool bindUdpAndReport(sf::UdpSocket & soc, unsigned short port);
    //bool sendUdpAndReport(sf::UdpSocket & soc, sf::Packet & packet, sf::IpAddress destinationIp, unsigned short destinationPort);
	//bool sendTcpAndReport(sf::TcpSocket & soc, sf::Packet & packet);//logs a little info about messages
    void processMessages(); // read through the incoming messages
    void handleMessage(PacketWrap & packetWrap);
    void greetClient(const PlayerConnected & newPlayer);
    void rejectClient(const PlayerConnected & toReject, const std::string & rejectionReason);
    void sendTcpToAll(sf::Packet & packet);

	void updateClients();
    bool getRunning()const
    {
        return running_;
    }
    bool running_;

    void createSomeTargets();
    typedef std::map<unsigned int,glm::vec2> Targets;
    Targets targets_;
    void checkTargets();
    void sendTargetList();
    void changeScore(Client & c, int score);
private:
	//!handling of messages
	void onPlayerAuthorize(const PlayerAuthorize & pe, const sf::IpAddress & ip, unsigned short port);
	void onPlayerDisconnect(const PlayerDisconnected & pe);
	void onPlayerUpdate(const PlayerStateUpdate & pe);

};

#endif // SERVER_H
