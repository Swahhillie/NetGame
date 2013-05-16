#ifndef NETWORKEVENTS_H_INCLUDED
#define NETWORKEVENTS_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <vector>
#include "KeyState.hpp"
class Client;

struct PacketWrap
{

    PacketWrap(const sf::Packet & pack, const sf::IpAddress & ip, unsigned short port):packet(pack),fromIp(ip),fromPort(port) {}

    sf::Packet packet;
    sf::IpAddress fromIp;
    unsigned short fromPort;
};
class Debug
{
public:
    static bool VERBOSE_MESSAGES;
    static bool SKIP_IP;
    static bool SKIP_PLAYER_NAME ;
    static bool SKIP_PLAYER_COLOR ;
    static bool REQUIRE_SERVER_CONNECTION;
    static bool DISCOVER_SERVERS;
    static bool AUTOFILL_LOGGIN;
};
//--------------------stringstream overloades
std::ostream & operator << (std::ostream & out, const sf::Color & color );
std::ostream & operator << (std::ostream & out, const glm::vec2 & v);

//---------------------packet operator overloads

//!sf::Color
sf::Packet & operator << (sf::Packet & packet, const sf::Color & color);
sf::Packet & operator >> (sf::Packet & packet, sf::Color & color);

//!sf::Ip
sf::Packet & operator << (sf::Packet & packet, const sf::IpAddress & ip);
sf::Packet & operator >> (sf::Packet & packet, sf::IpAddress & ip);

//!sf::vec2
sf::Packet & operator << (sf::Packet & packet, const glm::vec2 & v);
sf::Packet & operator >> (sf::Packet & packet, glm::vec2 & v);
//---------------------Network classes


/** \brief Some convenience functions
 *
 * \param
 * \param
 * \return
 * These functions will report the status of their action
 *
 */
/*--------------------------------------------------------------
//--base event
//--Provides interface for getting a command
//------------------------------------------------------------*/
namespace Network
{
//space for global functions that are of use to client server and masterserver
bool sendUdpAndReport(sf::UdpSocket & socket, sf::Packet & packet, const sf::IpAddress & address, unsigned short port);
bool bindUdpAndReport(sf::UdpSocket & socket, unsigned short port);
bool sendTcpAndReport(sf::TcpSocket & socket, sf::Packet & packet);
std::string commandToString(sf::Int32 command);
}

class NetworkEvent
{
public:
    enum Commands
    {
        PLAYER_CONNECTED,
        PLAYER_AUTHORIZE,
        PLAYER_DISCONNECTED,
        PLAYER_SEND_MESSAGE,
        PLAYER_REJECTED,
        PLAYER_ACCEPTED,
        PLAYER_UPDATE_TO_SERVER,
        PLAYER_UPDATE_TO_CLIENT,
        SERVER_ENTRY_GET,
        SERVER_ENTRY_ADD,
        SERVER_ENTRY_LIST,
        SERVER_ENTRY_ACCEPT,
        SERVER_ENTRY_REJECT,
        LOAD_LEVEL,
        CREATE_TARGET,
        DESTROY_TARGET,
        TARGET_LIST,
        SCORE_CHANGE
    };
    virtual sf::Int32 getCommand()const = 0;
private:

};
/** \brief Base Event for player events
 *
 * \param
 * \param
 * \return
 * This will will force all player related events to include a name
 */
/*--------------------------------------------------------------
//--base event for all player events
//--Provides interface for getting a commands that require a player to be specified
//------------------------------------------------------------*/
class PlayerEvent : public NetworkEvent
{

public:
    std::string name;
};
/** \brief player event connect
 *
 * \param
 * \param
 * \return
 *	Send by a connecting player to tell the server that he wants to connect.
 *  This message will later contain stuff like password / login vs register
 */

class PlayerConnected : public PlayerEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_CONNECTED;
    }
    sf::Color color;
    //sf::IpAddress ip;
    //unsigned short port;

    friend sf::Packet & operator << (sf::Packet &lhs, const PlayerConnected & rhs)
    {
        lhs << rhs.getCommand() << rhs.name << rhs.color;// << rhs.ip << rhs.port;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerConnected & rhs)
    {
        lhs >> rhs.name >> rhs.color;// >> rhs.ip >> rhs.port;
        return lhs;
    }
};
class LoadLevel : public NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::LOAD_LEVEL;
    }
    std::string levelName;

    friend sf::Packet & operator << (sf::Packet &lhs, const LoadLevel & rhs)
    {
        lhs << rhs.getCommand() << rhs.levelName;// << rhs.ip << rhs.port;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, LoadLevel & rhs)
    {
        lhs >> rhs.levelName;// >> rhs.ip >> rhs.port;
        return lhs;
    }
};
class PlayerAuthorize : public PlayerEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_AUTHORIZE;
    }
    std::string password;
    sf::IpAddress ip;
    unsigned short port;
    bool isRegistration;

    /*sf::Packet toPacket(){
    	sf::Packet packet;
    	return  packet << getCommand() << name << pw;
    }
    static PlayerAuthorize fromPacket(sf::Packet & packet){
    	PlayerAuthorize pa;
    	packet >> pa.name >> pa.pw;
    	return pa;
    }
    */

    friend sf::Packet & operator << (sf::Packet &lhs, const PlayerAuthorize & rhs)
    {
        lhs << rhs.getCommand() << rhs.name << rhs.password << rhs.ip << rhs.port << rhs.isRegistration;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerAuthorize & rhs)
    {
        lhs >> rhs.name >> rhs.password >> rhs.ip >> rhs.port >> rhs.isRegistration;
        return lhs;
    }
};

/** \brief player event disconnect
 *
 * \param
 * \param
 * \return
 * Ask the server to remove the client from the connected clients
 */

class PlayerDisconnected : public PlayerEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_DISCONNECTED;
    }
    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerDisconnected & rhs)
    {
        lhs << rhs.getCommand() << rhs.name;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerDisconnected & rhs)
    {
        lhs >> rhs.name;
        return lhs;
    }
};
/** \brief player event send message
 *
 * \param
 * \param
 * \return
 * Ask the server to broadcast a message to all connected players
 */

class PlayerSendMessage : public PlayerEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_SEND_MESSAGE;
    }
    std::string message;

    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerSendMessage & rhs)
    {
        lhs << rhs.getCommand() << rhs.name << rhs.message;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerSendMessage & rhs)
    {
        lhs >> rhs.name >> rhs.message;
        return lhs;
    }


};
/** \brief Event that is sent when the player get accepted to the server
 *
 * \param
 * \param
 * \return
 * has the server time so that the player can synch with it.
 */


class PlayerAccepted : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_ACCEPTED;
    }
    float serverTime;
    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerAccepted & rhs)
    {
        lhs << rhs.getCommand() << rhs.serverTime;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerAccepted & rhs)
    {
        lhs >> rhs.serverTime;
        return lhs;
    }

};
/*--------------------------------------------------------------
//--player event rejection
//--server rejects a player, for example when his username is used
//------------------------------------------------------------*/
class PlayerRejected : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_REJECTED;
    }
    std::string reason;
    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerRejected & rhs)
    {
        lhs << rhs.getCommand() << rhs.reason;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerRejected & rhs)
    {

        lhs >> rhs.reason;
        return lhs;
    }
};
/** \brief Holds info about where players currently are
 *
 * \return
 *
 */

class PlayerInfo
{
public:
    PlayerInfo & operator = (const Client & client);

    std::string name;
    glm::vec2 position;


    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerInfo & playerInfo)
    {
        return lhs << playerInfo.name << playerInfo.position;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerInfo & playerInfo)
    {
        return lhs >> playerInfo.name >> playerInfo.position;
    }

};

/** \brief Package containing info about all players connected to the server
 *
 *
 */

class PlayerStateUpdate : public PlayerEvent
{
public:
    PlayerStateUpdate():receiveTime(0) {}
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_UPDATE_TO_SERVER;
    }
    //float sendTime;
    KeyState keyState;
    mutable float receiveTime;
    glm::vec2 position; // ignored by server
    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerStateUpdate & rhs)
    {

        lhs << rhs.getCommand() << rhs.name << rhs.keyState << rhs.position;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerStateUpdate & rhs)
    {

        lhs >> rhs.name >> rhs.keyState >> rhs.position;
        return lhs;
    }


};

class PlayerUpdateToClient : NetworkEvent
{
public:
    PlayerUpdateToClient():serverTime(0), entryCount(0) {}

    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::PLAYER_UPDATE_TO_CLIENT;
    }

    float serverTime;
    sf::Uint32 entryCount;
    std::vector<PlayerStateUpdate> players;

    friend sf::Packet & operator << (sf::Packet & lhs, const PlayerUpdateToClient & rhs)
    {
        //adding the command and the amount of players this package will contain into the package
        assert(rhs.serverTime > 0.2f); //will cause problems if client is connected instantly
        assert(rhs.entryCount != 0);

        lhs << rhs.getCommand() << rhs.serverTime << rhs.entryCount;

        //adding all the playersInfos to the package
        for(unsigned int i = 0 ; i < rhs.entryCount; i++)
            lhs << rhs.players[i];

        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, PlayerUpdateToClient & rhs)
    {

        lhs >> rhs.serverTime >> rhs.entryCount;
        rhs.players = std::vector<PlayerStateUpdate>(rhs.entryCount); // allocate space for n players
        for(unsigned int i = 0; i < rhs.entryCount; i++)
        {
            sf::Int32 garbageInt; // don't need this int, it is for keeping the state
            lhs >> garbageInt >> rhs.players[i];
        }
        return lhs;
    }

};
/** \brief Update that is send from client to server
 *
 * \param
 * \param
 * \return
 * Contains the state of the keys
 */


//request for adding a server to the masterserver's server list
class ServerEntryAdd : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SERVER_ENTRY_ADD;
    }
    std::string name;
    sf::IpAddress ip;

    friend sf::Packet & operator << (sf::Packet & lhs, const ServerEntryAdd & rhs)
    {
        lhs << rhs.getCommand() << rhs.name << rhs.ip;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, ServerEntryAdd & rhs)
    {
        lhs >> rhs.name >> rhs.ip;
        return lhs;
    }

};
//Request for server entries
class ServerEntryGet : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SERVER_ENTRY_GET;
    }

    friend sf::Packet & operator << (sf::Packet & lhs, const ServerEntryGet & rhs)
    {
        lhs << rhs.getCommand();
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, ServerEntryGet & rhs)
    {
        return lhs;
    }
};
//an event that tells a server that its attempt to create a server entry has succeeded
class ServerEntryAccept : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SERVER_ENTRY_ACCEPT;
    }

    friend sf::Packet & operator << (sf::Packet & lhs, const ServerEntryAccept & rhs)
    {
        lhs << rhs.getCommand();
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, ServerEntryAccept & rhs)
    {
        return lhs;
    }
};

//An event that tells a server that its attempt to create a server entry has failed
class ServerEntryReject : NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SERVER_ENTRY_REJECT;
    }
    std::string reason;
    friend sf::Packet & operator << (sf::Packet & lhs, const ServerEntryReject & rhs)
    {
        lhs << rhs.getCommand() << rhs.reason;
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, ServerEntryReject & rhs)
    {
        lhs >> rhs.reason;
        return lhs;
    }

};

//Class for storing information about servers

class ServerEntry
{
public:
    std::string name;
    sf::IpAddress ip;

    friend sf::Packet & operator << (sf::Packet & packet, const ServerEntry & entry)
    {
        return packet << entry.name << entry.ip;
    }
    friend sf::Packet & operator >> (sf::Packet & packet, ServerEntry & entry)
    {
        return packet >> entry.name >> entry.ip;
    }
};
//serializable server entry container
class ServerEntryList : NetworkEvent
{
public:

    unsigned int entryCount;
    std::vector<ServerEntry> entries;

    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SERVER_ENTRY_LIST;
    }
    friend sf::Packet & operator << (sf::Packet & lhs, const ServerEntryList & rhs)
    {
        assert(rhs.entryCount >= 0 && rhs.entryCount < MAX_ENTRIES);
        lhs << rhs.getCommand() << rhs.entryCount;
        for(unsigned int i = 0; i < rhs.entryCount; i++) lhs << rhs.entries[i];
        return lhs;
    }
    friend sf::Packet & operator >> (sf::Packet & lhs, ServerEntryList & rhs)
    {

        //
        lhs >> rhs.entryCount;
        rhs.entries = std::vector<ServerEntry>(rhs.entryCount);
        for(unsigned int i = 0 ; i < rhs.entryCount; i++)lhs >> rhs.entries[i];
        return lhs;
    }
    std::string getFormattedSting()const
    {
        std::stringstream stream;
        stream << "__\bServer Entries\b__" << std::endl;;
        for(unsigned int i = 0; i < entryCount; i++)
        {
            const ServerEntry & entry = entries[i];
            stream << i << ":" << std::endl;
            stream << entry.name << std::endl;
            stream << entry.ip.toString() << std::endl;
            stream << "- - - - - - - - - -" << std::endl;
        }
        stream << "___________________";
        return stream.str();
    }
private:
    static const unsigned int MAX_ENTRIES = 10;
};
/** \brief Send list of targets in one go.
*
 */

class TargetList : public NetworkEvent
{
	public:
	TargetList():entryCount(0){}

	virtual sf::Int32 getCommand()const{
		return NetworkEvent::TARGET_LIST;
	}
	unsigned int entryCount;
	std::vector<unsigned int> targetIds;
    std::vector<glm::vec2> targetPositions;

    friend sf::Packet & operator << (sf::Packet & packet, const TargetList & targets)
    {

		packet << targets.getCommand();
		packet << targets.entryCount;
		assert(targets.targetIds.size() == targets.entryCount);
		assert(targets.targetPositions.size() == targets.entryCount);
		for(unsigned int i = 0 ; i < targets.entryCount; i++){
			packet << targets.targetIds[i] << targets.targetPositions[i];
		}
		return packet;
    }
    friend sf::Packet & operator >> (sf::Packet & packet, TargetList & targets)
    {
		packet >> targets.entryCount;
		targets.targetIds = std::vector<unsigned int>(targets.entryCount);
		targets.targetPositions = std::vector<glm::vec2>(targets.entryCount);
		for(unsigned int i = 0; i < targets.entryCount; i++){
			packet >> targets.targetIds[i] >> targets.targetPositions[i];

		}
		return packet;
    }
};
/** \brief Command to create one target at position for the client
 *
 */

class CreateTarget : public NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::CREATE_TARGET;
    }

    unsigned int targetId;
    glm::vec2 position;

    friend sf::Packet & operator << (sf::Packet & packet, const CreateTarget & tar)
    {
        return packet << tar.getCommand() << tar.targetId << tar.position;
    }
    friend sf::Packet & operator >> (sf::Packet & packet, CreateTarget & tar)
    {
        return packet >> tar.targetId >> tar.position;
    }
};
class DestroyTarget : public NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::DESTROY_TARGET;
    }

    unsigned int targetId;

    friend sf::Packet & operator << (sf::Packet & packet, const DestroyTarget & tar)
    {
        return packet << tar.getCommand() << tar.targetId;
    }
    friend sf::Packet & operator >> (sf::Packet & packet, DestroyTarget & tar)
    {
        return packet >> tar.targetId;
    }
};
struct ScoreObject{
	std::string name;
	int score;
	friend sf::Packet & operator << (sf::Packet & packet, const ScoreObject & scoreObject){
		return  packet << scoreObject.name << scoreObject.score;
	}
	friend sf::Packet & operator >> (sf::Packet & packet,  ScoreObject & scoreObject){
		return  packet >> scoreObject.name >> scoreObject.score;
	}
};

class ScoreChange : public NetworkEvent
{
public:
    virtual sf::Int32 getCommand()const
    {
        return NetworkEvent::SCORE_CHANGE;
    }

    unsigned int entryCount;
    std::vector<ScoreObject> scoreObjects;

    friend sf::Packet & operator << (sf::Packet & packet, const ScoreChange & tar)
    {
        packet<< tar.getCommand() << tar.entryCount;
        for(unsigned int i = 0; i < tar.entryCount; i++)
			packet << tar.scoreObjects[i];

        return packet;
    }
    friend sf::Packet & operator >> (sf::Packet & packet, ScoreChange & tar)
    {
		packet >> tar.entryCount;
		tar.scoreObjects = std::vector<ScoreObject>(tar.entryCount);
		for(unsigned int i = 0; i < tar.entryCount; i++)
			packet >> tar.scoreObjects[i];

        return packet;
    }
};


#endif // NETWORKEVENTS_H_INCLUDED
