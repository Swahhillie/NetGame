#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "FullExpressionAccumulator.hpp"
#include "NetworkEvents.h"

class CommandHandler
{
	public:
		CommandHandler();
		virtual ~CommandHandler();

		void handlePacket(PacketWrap & packwrap);



	protected:

		//these can be overloaded to supply the functionality needed by a specific implementation of the handler
		virtual void player_connected(const PlayerConnected & pe)				{notImplemented();}
		virtual void player_authorize(const PlayerAuthorize & pe, const sf::IpAddress & fromIp, unsigned short port)				{notImplemented();}
		virtual void player_disconnected(const PlayerDisconnected & pe)			{notImplemented();}
		virtual void player_send_message(const PlayerSendMessage & pe)			{notImplemented();}
		virtual void player_rejected(const PlayerRejected & pe)					{notImplemented();}
		virtual void player_accepted(const PlayerAccepted & pe)					{notImplemented();}
		virtual void player_update_to_server(const PlayerStateUpdate & pe)	{notImplemented();}
		virtual void player_update_to_client(const PlayerUpdateToClient & pe)	{notImplemented();}
		virtual void server_entry_get(const ServerEntryGet & pe)				{notImplemented();}
		virtual void server_entry_add(const ServerEntryAdd & pe)				{notImplemented();}
		virtual void server_entry_list(const ServerEntryList & pe)				{notImplemented();}
		virtual void server_entry_accept(const ServerEntryAccept & pe)			{notImplemented();}
		virtual void server_entry_reject(const ServerEntryReject & pe)			{notImplemented();}
		virtual void load_level(const LoadLevel & pe)			{notImplemented();}
		virtual void destroy_target(const DestroyTarget & pe)			{notImplemented();}
		virtual void score_change(const ScoreChange & pe)			{notImplemented();}
		virtual void create_target(const CreateTarget & pe)			{notImplemented();}
	private:

		void notImplemented(){
			FullExpressionAccumulator(std::cout) << "Command handler doesn't expect this command\n";
		}
		bool isSilent(sf::Int32 commands);

};

#endif // COMMANDHANDLER_H
