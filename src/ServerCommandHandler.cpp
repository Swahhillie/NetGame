#include "ServerCommandHandler.hpp"
#include "Server.hpp"
#include "FullExpressionAccumulator.hpp"
#include "Time.hpp"

ServerCommandHandler::ServerCommandHandler(Server & server):
	server_(server)
{
    //ctor
}

ServerCommandHandler::~ServerCommandHandler()
{
    //dtor
}
//void ServerCommandHandler::player_connected(const PlayerConnected & pe)
//{
//	server_.onPlayerConnect(pe);
//}

void ServerCommandHandler::player_authorize(const PlayerAuthorize & pe, const sf::IpAddress & ip, unsigned short port){
	server_.onPlayerAuthorize(pe, ip, port);
}
void ServerCommandHandler::player_disconnected(const PlayerDisconnected & pe)
{
	server_.onPlayerDisconnect(pe);
}
void ServerCommandHandler::player_send_message(const PlayerSendMessage & pe)
{
	FullExpressionAccumulator(std::cout) << "Server received message from player '" << pe.name << ":" << pe.message << "\n";
	//should echo to all clients
}
void ServerCommandHandler::server_entry_accept(const ServerEntryAccept & pe)
{
	FullExpressionAccumulator(std::cout) << "Server entry was accepted, jippie \n";
}
void ServerCommandHandler::server_entry_reject(const ServerEntryReject & pe)
{
	FullExpressionAccumulator(std::cout) << "Server entry was rejected for reason: " << pe.reason << "\n";
}
void ServerCommandHandler::player_update_to_server(const PlayerStateUpdate & pe)
{
	pe.receiveTime = Time::time;
	server_.onPlayerUpdate(pe);
}
