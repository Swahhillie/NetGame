#ifndef SERVERCOMMANDHANDLER_H
#define SERVERCOMMANDHANDLER_H

#include "CommandHandler.hpp"
class Server;

class ServerCommandHandler : public CommandHandler
{
	public:
		ServerCommandHandler(Server & server);
		virtual ~ServerCommandHandler();
	protected:
		//virtual void player_connected(const PlayerConnected & pe);
		virtual void player_authorize(const PlayerAuthorize & pe, const sf::IpAddress & fromIp, unsigned short fromPort);
		virtual void player_disconnected(const PlayerDisconnected & pe);
		virtual void player_send_message(const PlayerSendMessage & pe);
		virtual void server_entry_accept(const ServerEntryAccept & pe);
		virtual void server_entry_reject(const ServerEntryReject & pe);
		virtual void player_update_to_server(const PlayerStateUpdate & pe);
	private:
		Server & server_;
};

#endif // SERVERCOMMANDHANDLER_H
