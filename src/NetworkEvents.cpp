#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <GLM/GLM.hpp>
#include <sstream>
#include <NetworkEvents.h>
#include <DebugSpace.h>
#include "FullExpressionAccumulator.hpp"
#include "Client.hpp"

bool Debug::VERBOSE_MESSAGES;
bool Debug::SKIP_IP;
bool Debug::SKIP_PLAYER_NAME ;
bool Debug::SKIP_PLAYER_COLOR ;
bool Debug::REQUIRE_SERVER_CONNECTION ;
bool Debug::DISCOVER_SERVERS;
bool Debug::AUTOFILL_LOGGIN;

std::ostream & operator << (std::ostream & out, const sf::Color & color )
{
    out << '(' << +color.r << "," << +color.g << "," << +color.b << ')';
    return out;
}

std::ostream & operator << (std::ostream & out, const glm::vec2 & v){
	return out << "(" << v.x << "," << v.y << ")";
}
//!sf::Color
sf::Packet & operator << (sf::Packet & packet, const sf::Color & color)
{
    return packet << color.r << color.g << color.b << color.a;
}
sf::Packet & operator >> (sf::Packet & packet, sf::Color & color)
{
    return packet >> color.r >> color.g >> color.b >> color.a;
}

//!sf::Ip
sf::Packet & operator << (sf::Packet & packet, const sf::IpAddress & ip)
{
    return packet << ip.toInteger();
}
sf::Packet & operator >> (sf::Packet & packet, sf::IpAddress & ip)
{
    sf::Uint32 ipAsUint;
    packet >> ipAsUint;
    ip = sf::IpAddress(ipAsUint);
    return packet;
}


//!glm::vec2
sf::Packet & operator << (sf::Packet & packet, const glm::vec2 & v)
{
    return packet << v.x << v.y;
}
sf::Packet & operator >> (sf::Packet & packet, glm::vec2 & v)
{
    return packet >> v.x >> v.y;
}

bool Network::sendUdpAndReport(sf::UdpSocket & soc, sf::Packet & packet,const sf::IpAddress & destinationIp, unsigned short destinationPort)
{
    sf::Socket::Status status = soc.send(packet, destinationIp, destinationPort);
    switch(status)
    {
    case sf::Socket::Done:
        //if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Succesfully send  udp packet to "
          //          <<destinationIp.toString() << " : " << destinationPort << "\n";
        break;
    case sf::Socket::Disconnected:
        FullExpressionAccumulator(std::cout) << "Failed sending packet to "
                                             <<destinationIp.toString() << " : " << destinationPort << ", reason: udp socket = DISCONNECTED\n";
        break;
    case sf::Socket::NotReady:
        FullExpressionAccumulator(std::cout) << "Failed sending packet to "
                                             <<destinationIp.toString() << " : " << destinationPort << ", reason: udp socket = NotReady\n";
        break;
    case sf::Socket::Error:
        FullExpressionAccumulator(std::cout) << "Failed sending packet to "
                                             <<destinationIp.toString() << " : " << destinationPort << ", reason: udp socket = Error\n";
        break;
    }
    return status == sf::Socket::Done;
}
bool Network::sendTcpAndReport(sf::TcpSocket & socket, sf::Packet & packet)
{
    sf::Socket::Status result = socket.send(packet);

    switch(result)
    {
    case sf::Socket::Done:
        if(Debug::VERBOSE_MESSAGES && false)FullExpressionAccumulator(std::cout) << "Send tcp success\n";
        break;
    case sf::Socket::Disconnected:
        FullExpressionAccumulator(std::cout) << "Failed to send because Tcp socket is disconnected\n";
        break;
    case sf::Socket::Error:
        FullExpressionAccumulator(std::cout) << "Failed to send because Tcp socket has errors\n";
        break;
    case sf::Socket::NotReady:
        FullExpressionAccumulator(std::cout) << "Failed to send because Tcp socket is not ready\n";
        break;
    }
    return result == sf::Socket::Done;
}
bool Network::bindUdpAndReport(sf::UdpSocket & soc, unsigned short port)
{
    sf::Socket::Status result = soc.bind(port);
    assert(result == sf::Socket::Done); // SERVERPORT is already bound to something, a server is probably already running on this ip
    //implement gracefull failure later
    return result == sf::Socket::Done;

}
std::string Network::commandToString(sf::Int32 command)
{
    std::stringstream commandString;
    commandString << "[";
    switch(command)
    {
    case NetworkEvent::PLAYER_CONNECTED:
        commandString << "PLAYER_CONNECTED";
        break;
	case NetworkEvent::PLAYER_AUTHORIZE:
        commandString << "PLAYER_AUTHORIZE";
        break;
    case NetworkEvent::PLAYER_DISCONNECTED:
        commandString << "PLAYER_DISCONNECTED";
        break;
    case NetworkEvent::PLAYER_SEND_MESSAGE:
        commandString << "PLAYER_SENDMESSAGE";
        break;
    case NetworkEvent::PLAYER_REJECTED:
        commandString << "PLAYER_REJECTED";
        break;
    case NetworkEvent::PLAYER_ACCEPTED:
        commandString << "PLAYER_ACCEPTED";
        break;
    case NetworkEvent::PLAYER_UPDATE_TO_SERVER:
        commandString << "PLAYER_UPDATE_TO_SERVER";
        break;
    case NetworkEvent::PLAYER_UPDATE_TO_CLIENT:
        commandString << "PLAYER_UPDATE_TO_CLIENT";
        break;
    case NetworkEvent::SERVER_ENTRY_GET:
        commandString << "SERVER_ENTRY_GET";
        break;
    case NetworkEvent::SERVER_ENTRY_ADD:
        commandString << "SERVER_ENTRY_ADD";
        break;
    case NetworkEvent::SERVER_ENTRY_LIST:
        commandString << "SERVER_ENTRY_LIST";
        break;
    case NetworkEvent::SERVER_ENTRY_ACCEPT:
        commandString << "SERVER_ENTRY_ACCEPT";
        break;
    case NetworkEvent::SERVER_ENTRY_REJECT:
        commandString << "SERVER_ENTRY_REJECT";
        break;
	case NetworkEvent::LOAD_LEVEL:
        commandString << "LOAD_LEVEL";
        break;
	case NetworkEvent::CREATE_TARGET:
        commandString << "CREATE_TARGET";
        break;
	case NetworkEvent::DESTROY_TARGET:
        commandString << "DESTROY_TARGET";
        break;
	case NetworkEvent::TARGET_LIST:
        commandString << "TARGET_LIST";
        break;
	case NetworkEvent::SCORE_CHANGE:
        commandString << "SCORE_CHANGE";
        break;
    default:
        commandString << command << " was not specified!";
        break;
    }
    commandString << "]";
    return commandString.str();
}

PlayerInfo & PlayerInfo::operator = (const Client & c){
	this->name = c.name;
	this->position = c.position;
	return *this;
}

