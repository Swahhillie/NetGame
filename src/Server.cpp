#include "Server.hpp"
#include "FullExpressionAccumulator.hpp"
#include "Global_Constants.hpp"
#include "Mutexes.hpp"
#include <assert.h>

#include "Time.hpp"
#include "NetworkEvents.h"
#include "ConfigLoader.hpp"
#include "DebugSpace.h"
#include "Client.hpp"
#include "Level.hpp"
#include "Character.hpp"
#include <glm/gtx/norm.hpp>
#include <math.h>

Server::Server():
    clientsManager_(true),
    serverCommandHandler_(*this),
    tcpReceiver(&Server::listenSocketSelector, this),
    udpReceiver(&Server::listenUdp,this)

{
    //ctor
    running_ = true;
    updatesPerSecond = ConfigLoader::instance().getFloat("settings,server,updatesPerSecond");
    std::cout << "Server updates " << updatesPerSecond << " times per second, or every" << (1.0f / updatesPerSecond) << " seconds\n";
    assert(updatesPerSecond < 30); // dont go crazy
    //!setting up the socketselector




}

Server::~Server()
{
    //dtor

    running_ = false;

    //iterate of the tcp sockets and destroy them
    for(TcpConnections::iterator it = tcpConnections.begin(); it != tcpConnections.end(); ++it)
    {
        sf::TcpSocket * conn = *it;
        delete conn;
    }
    tcpConnections.clear();


    FullExpressionAccumulator(std::cout) << "SERVER CLOSED \n";
}
void Server::build()
{
    //setting up the server for
    tcpReceiver.launch();
    udpReceiver.launch();
    serverLoadLevel("rectangle");

    //binding the udp socket



}
void Server::serverLoadLevel(const std::string & name)
{
    FullExpressionAccumulator(std::cout) << "Loaded level " << name << " on server \n";
    lvl_.loadLevelNamed(name);
    clientsManager_.setActiveLevel(&lvl_);
    LoadLevel loadLevel;
    loadLevel.levelName = name;
    sf::Packet lvlChangePack;
    lvlChangePack << loadLevel;
	sendTcpToAll(lvlChangePack);


    createSomeTargets();
	sendTargetList();
    //score reset
    clientsManager_.resetScores();
    ScoreChange sc = clientsManager_.getScores();
    sf::Packet scPack;
    scPack << sc;
    sendTcpToAll(scPack);





}
void Server::listenSocketSelector()
{
    FullExpressionAccumulator(std::cout) << "Server::listenSocketSelector() STARTED \n";
    listener_.listen(SERVERPORT);
    selector_.add(listener_);

    while(getRunning())
    {

        //if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Ready to receive\n";
        if(selector_.wait(sf::seconds(2.0f)))
        {

            //if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Network Event occured\n";
            if(selector_.isReady(listener_))
            {
                //the listener_ picked up a new connection. go and accept it
                acceptNewTcpConnection(listener_);
            }
            else
            {
                //that leaves the tcp sockets to be checked.
                readMessagesFromTcpSockets();

            }

        }
        else
        {
            if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "selector timed out\n";
        }

    }


    FullExpressionAccumulator(std::cout) << "Server::listenSocketSelector() ENDED\n";
}
void Server::listenUdp()
{
    FullExpressionAccumulator(std::cout) << "Server::listenUdp() STARTED\n";

    //binding the udpsocket
    if(Network::bindUdpAndReport(udpSocket_, SERVERPORT))
    {
        if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Successfully bound udp socket to port " << SERVERPORT << "\n";
        requestServerEntryAdd();


        while(getRunning())
        {

            sf::Packet receivedPacket;
            sf::IpAddress remoteIp;
            unsigned short remotePort;
            sf::Socket::Status status = udpSocket_.receive(receivedPacket, remoteIp, remotePort);
            if(status == sf::Socket::Done)
            {
                if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Succes receiving a udp packet, queing it for reading \n";
                addMessageToRead(receivedPacket, remoteIp, remotePort);

            }
            else
            {
                FullExpressionAccumulator(std::cout) << "Failed receiving a udp packet with status " << status << " \n";
            }
        }
        FullExpressionAccumulator(std::cout) << "Server::listenUdp() ENDED\n";

    }


    else
    {
        FullExpressionAccumulator(std::cout) << "Could not bind udp socket to port " << SERVERPORT << "\n";
    }



}
void Server::run()
{
    FullExpressionAccumulator(std::cout) << "Server::run() STARTED\n" ;

    while(getRunning())
    {
        Time::update();
        processMessages();
        updateClients();
        sendStateUpdates();
        checkTargets();
        float sleepForSeconds = 1 / updatesPerSecond;// - Time::deltaTime; //maintain fps
        //std::cout << "sleeping for " << sleepForSeconds <<"\n";
        sf::sleep(sf::seconds(sleepForSeconds));

    }

    FullExpressionAccumulator(std::cout) << "Server::run() ENDED\n";

}
void Server::requestServerEntryAdd()
{
    //for(int i = 0 ; i < 10; i++){
    sf::Packet entryAddPacket;
    ServerEntryAdd entryAddEvent;
    std::stringstream counterStream;
//		counterStream << i;
    //	counterStream >> entryAddEvent.name;
    entryAddEvent.name = "  Test server, Please ignore ";
    entryAddEvent.ip = sf::IpAddress::getPublicAddress();
    entryAddPacket << entryAddEvent;
    Network::sendUdpAndReport(udpSocket_, entryAddPacket, sf::IpAddress(ConfigLoader::instance().getMasterServerAddress()), MASTERSERVERPORT);
    //}

}
void Server::updateClients()
{
    //extrapolate the position of each client based on its keystate and last known velocity and time

    clientsManager_.extrapolatePositions();

    //construct a udp packet with the predicted position of each client



    //send the packet to all connected clients
}
void Server::sendStateUpdates()
{
    const ClientsManager::Clients & clients = clientsManager_.getClients();

    if(clients.size() > 0)
    {
        sf::Packet pack = clientsManager_.getClientUpdates();
        for(ClientsManager::Clients::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            const Client & c = *(it->second);
            Network::sendUdpAndReport(udpSocket_, pack, c.ip, c.port);
        }
    }

    //return
    /*
    if(clientAddresses.size() != 0){

    	for(std::vector<sf::IpAddress>::const_iterator it = clientAddresses.begin(); it != clientAddresses.end(); ++it){
    		Network::sendUdpAndReport(udpSocket_, pack, *it, CLIENTPORT);
    	}
    }
    else{

    	//there are no clients connected to send packets to, dont bother creating them
    }
    */

}
void Server::acceptNewTcpConnection(sf::TcpListener & listener)
{

    sf::TcpSocket * connection = new sf::TcpSocket;

    if(listener.accept(*connection) == sf::Socket::Done)
    {

        //hold on to the client for memory purpose
        tcpConnections.push_back(connection);

        //place in the selector for listening
        selector_.add(*connection);

        FullExpressionAccumulator(std::cout) << "Tcp Client "<< connection->getRemoteAddress().toString() << " has connected\n";

    }
    else
    {
        FullExpressionAccumulator(std::cout) << "Tcp Client "<< connection->getRemoteAddress().toString() << " failed to connect\n";
        delete connection; // clean up the failed connection
    }

}
void Server::readMessagesFromTcpSockets()
{
    bool wasReady = false;
    for(TcpConnections::iterator it = tcpConnections.begin(); it != tcpConnections.end(); ++it)
    {
        sf::TcpSocket & connection = **it; //derefernce iterator and pointer

        if(selector_.isReady(connection)) // this is the client that has received new data.
        {
            wasReady = true;
            sf::Packet receivedPacket;

            sf::Socket::Status status = connection.receive(receivedPacket);
            if(status == sf::Socket::Done)
            {
                addMessageToRead(receivedPacket, connection.getRemoteAddress(), connection.getRemotePort());
            }
            else if(status == sf::Socket::Disconnected)
            {
                FullExpressionAccumulator(std::cout) << "Tcp Client "<< connection.getRemoteAddress() << " has disconnected\n";

                it = tcpConnections.erase(it)--; // go to the previous element and continue from there
                selector_.remove(connection);
                delete &connection; // cleanup the now disconnected connection

            }
            else if(status == sf::Socket::NotReady)
            {
                FullExpressionAccumulator(std::cout) << "Tcp Client is not ready\n";
            }
            else if(status == sf::Socket::Error)
            {
                FullExpressionAccumulator(std::cout) << "Tcp socket has errored\n";
            }
        }

    }
    assert(wasReady == true);
}

void Server::readMessageFromUdpSocket(sf::UdpSocket & socket)
{

}
void Server::addMessageToRead(const sf::Packet & packet, const sf::IpAddress & remoteAddress, unsigned short remotePort)
{
    //copy the packet into the to read queue

    Mutexes::messages.lock();//grab exclusive excess to the queue

    toRead.push(PacketWrap(packet,remoteAddress,remotePort));

    Mutexes::messages.unlock();
}
void Server::processMessages()
{

    while(!toRead.empty())
    {
        Mutexes::messages.lock();
        PacketWrap pack = toRead.front();
        toRead.pop();
        Mutexes::messages.unlock();

        handleMessage(pack);
    }

}

void Server::handleMessage(PacketWrap & packetWrap)
{

    serverCommandHandler_.handlePacket(packetWrap);
    /*sf::Int32 command;
    bool isFailedToWrite = false;



    packet >> command;

    if(Debug::VERBOSE_MESSAGES)
        FullExpressionAccumulator(std::cout) << "Received "<< Network::commandToString(command) << " command \n";

    switch(command)
    {
    case NetworkEvent::PLAYER_CONNECTED:
        onPlayerConnect(packet);

        break;
    case NetworkEvent::PLAYER_DISCONNECTED:
        onPlayerDisconnect(packet);
        break;
    case NetworkEvent::PLAYER_UPDATE_TO_SERVER:
    	onPlayerUpdate(packet);
    	break;
    case NetworkEvent::PLAYER_SEND_MESSAGE:

        break;
    }

    if(Debug::VERBOSE_MESSAGES)
        if(isFailedToWrite)
            FullExpressionAccumulator(std::cout) << "Failed to write to " << Network::commandToString(command) << " event class \n";
    */
}

void Server::onPlayerAuthorize(const PlayerAuthorize & authorizeMessage, const sf::IpAddress & rIp, unsigned short rPort)
{
    PlayerConnected playerConnectedEvent;
    playerConnectedEvent.name = authorizeMessage.name;
    playerConnectedEvent.color = sf::Color::Red;
    std::string rejectionReason;

    if(authorizeMessage.isRegistration)
    {
        if(databaseManager_.registerUser(authorizeMessage.name, authorizeMessage.password, rejectionReason))
        {
            /*if(clientsManager_.addClient(authorizeMessage.name, sf::Color::Green, rIp, authorizeMessage.port))
            {
                FullExpressionAccumulator(std::cout) << "Player: " << authorizeMessage.name << " , with ip " << authorizeMessage.ip.toString() << " : " << authorizeMessage.port <<" has connected\n";
                //client has succesfully added himself, tell the world about it.


                greetClient(playerConnectedEvent);
            }
            else
            {
                //this client could not be added because the clientmanager rejected it
                //tell him to kindly try again
                rejectClient(playerConnectedEvent, "Registration successfull but login failed because " + clientsManager_.getRejectionReason());
            */
            rejectClient(playerConnectedEvent, rejectionReason);//make the player perform a login
        }
        else
        {
            rejectClient(playerConnectedEvent, "Registration failed because: " + rejectionReason);
        }


    }
    else
    {
        if (databaseManager_.login(authorizeMessage.name, authorizeMessage.password, rejectionReason))
        {
            if(clientsManager_.addClient(authorizeMessage.name, sf::Color::Green, rIp, authorizeMessage.port))
            {
                FullExpressionAccumulator(std::cout) << "Player: " << authorizeMessage.name << " , with ip " << authorizeMessage.ip.toString() << " : " << authorizeMessage.port <<" has connected\n";
                //client has succesfully added himself, tell the world about it.


                greetClient(playerConnectedEvent);
            }
            else
            {
                //this client could not be added because the clientmanager rejected it
                //tell him to kindly try again
                rejectClient(playerConnectedEvent, "Login success but join failed because : " + clientsManager_.getRejectionReason());
            }
        }
        else
        {
            rejectClient(playerConnectedEvent,"Login failed because: " + rejectionReason);

        }

    }


}
void Server::onPlayerDisconnect(const PlayerDisconnected & playerDisconnectedEvent)
{

    clientsManager_.removeClient(playerDisconnectedEvent.name);
    sf::Packet packet;
    packet << playerDisconnectedEvent;
    sendTcpToAll(packet);
}
void Server::onPlayerUpdate(const PlayerStateUpdate & playerUpdate)
{
    clientsManager_.updateClient(playerUpdate);
}
void Server::greetClient(const PlayerConnected & playerConnectedEvent)
{
    sf::Packet acceptPacket;
    PlayerAccepted playerAcceptedEvent;
    playerAcceptedEvent.serverTime = Time::time;
    acceptPacket << playerAcceptedEvent;


    //accept the new connection connection
    assert(!tcpConnections.empty());
    Network::sendTcpAndReport(*tcpConnections.back(), acceptPacket);

    //instruct the client to load the level the server is currently playing
    LoadLevel loadLevelEvent;
    loadLevelEvent.levelName = clientsManager_.getActiveLevel().getLoadedLevelName();
    sf::Packet loadLevelPacket;
    loadLevelPacket << loadLevelEvent;
    Network::sendTcpAndReport(*tcpConnections.back(), loadLevelPacket);

    //send information about all the currently connected clients
    ClientsManager::Clients & clients = clientsManager_.getClients();
    for(ClientsManager::Clients::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        const Client&c = *it->second;
        if(c.name == playerConnectedEvent.name)continue;
        sf::Packet bringUpToDatePacket;
        PlayerConnected pc;
        pc.name = c.name;
        pc.color = c.color;
        bringUpToDatePacket << pc;
        Network::sendTcpAndReport(*tcpConnections.back(), bringUpToDatePacket);
    }

    //send information about all targets currently in the field
	sendTargetList();

    //send all the other clients connected to the server information about the new connection
    sf::Packet connectionPacket;
    connectionPacket << playerConnectedEvent;

    FullExpressionAccumulator(std::cout) << "Sending client connected event to " << tcpConnections.size() <<" Clients \n";


    int i = 0;
    for( TcpConnections::iterator it = tcpConnections.begin(); it != tcpConnections.end(); ++it, ++i)
    {
        sf::TcpSocket & connection= **it;
        Network::sendTcpAndReport(connection, connectionPacket);
    }
}
void Server::sendTargetList()
{
    TargetList tl;
    tl.entryCount = targets_.size();
    tl.targetPositions = std::vector<glm::vec2>(targets_.size());
    tl.targetIds = std::vector<unsigned int>(targets_.size());
    int i = 0;
    for( Targets::iterator it = targets_.begin(); it != targets_.end(); ++it, ++i)
    {
        tl.targetIds[i] = it->first;
        tl.targetPositions[i] = it->second;
    }
    sf::Packet targetListPacket;
    targetListPacket << tl;
    Network::sendTcpAndReport(*tcpConnections.back(), targetListPacket);
}
void Server::rejectClient(const PlayerConnected & playerConnectedEvent, const std::string & reason)
{
    //create the event to send to the player
    sf::Packet rejectionPacket;
    PlayerRejected rejectionEvent;
    rejectionEvent.reason = reason;
    rejectionPacket << rejectionEvent;

    assert(!tcpConnections.empty());
    Network::sendTcpAndReport(*tcpConnections.back(), rejectionPacket);


    return;

    /*//find the tcp connection that belongs to this client,
    //normally use the clients map but because this a rejection the client is not in there.
    for(TcpConnections::iterator it = tcpConnections.begin(); it != tcpConnections.end(); ++it)
    {
        sf::TcpSocket & connection = **it;
        if(connection.getRemoteAddress() == playerConnectedEvent.ip)
        {
            FullExpressionAccumulator(std::cout) << "Sending rejection packet to " << playerConnectedEvent.ip.toString() << "\n";
            //currently just disconnects the first localhost it comes across. ip will not be the same normally
            Network::sendTcpAndReport(connection, rejectionPacket);
            return;
        }
    }
    */

}
void Server::createSomeTargets()
{
    unsigned int spread = 300;

    unsigned int amount = 5;
//    float spacing = spread / amount;

    assert(targets_.size() == 0);
    const std::vector<glm::vec2> & ts = lvl_.getObjectPositions();
    for(unsigned int i = 0; i < ts.size(); ++i)
    {
        targets_[i] = ts[i];
    }


}
void Server::checkTargets()
{
    float radius = 20;
    ClientsManager::Clients & clients = clientsManager_.getClients();
    ClientsManager::Clients::iterator it = clients.begin();
    //  float closestPoint = 1000000.0f;

    for(; it != clients.end(); ++it)
    {

        Client & client = *it->second;
        Character & ch = *it->second->character;

        Targets::iterator jt = targets_.begin();
        for(; jt != targets_.end(); ++jt)
        {
            glm::vec2 t = jt->second;

            float distance2  = glm::length2(ch.getPosition() - t);
            if(distance2 < radius * radius)
            {
                //close enough

                std::cout << "client " << client.name << "picked up target " << jt->first << std::endl;
                DestroyTarget dt;
                dt.targetId = jt->first;
                sf::Packet pack;
                pack << dt;
                sendTcpToAll(pack);
                changeScore(client, 20);
                targets_.erase(jt);
                break;

            }
        }

        if(targets_.size() == 0)
        {
            if(lvl_.getLoadedLevelName() == "rectangle")
            {
                serverLoadLevel("lvl2");
            }
            else
            {
                serverLoadLevel("rectangle");
            }
        }
        /*
        for(int i = targets_.size(); i >= 0; i--)
        {
            glm::vec2 t = targets_[i];

            Character & c = *it->second->character;
            float distance2  = glm::length2(c.getPosition() - t);
            if(distance2 < radius * radius)
            {
                //close enough
                targets_.erase(targets_.begin() + i);

                DestroyTarget dt;
                dt.targetId = i;
                sf::Packet pack;
                pack << dt;
                sendTcpToAll(pack);

            }
            //  if(distance2 < closestPoint) closestPoint = distance2;


        }
        */
    }

}
void Server::changeScore(Client & client, int score)
{
    client.score += score;
    const ScoreChange sc = clientsManager_.getScores();
    sf::Packet packet;
    packet << sc;
    sendTcpToAll(packet);
}
void Server::sendTcpToAll(sf::Packet & pack)
{
    TcpConnections::iterator it = tcpConnections.begin();
    for(; it != tcpConnections.end(); ++it)
    {
        sf::TcpSocket & connection = **it;
        Network::sendTcpAndReport(connection, pack);

    }
}
