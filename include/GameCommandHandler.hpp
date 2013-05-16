#ifndef GAMECOMMANDHANDLER_H
#define GAMECOMMANDHANDLER_H

#include "CommandHandler.hpp"

class Game;

class GameCommandHandler : public CommandHandler
{
	public:
		GameCommandHandler(Game & game);
		virtual ~GameCommandHandler();

	protected:
		virtual void player_update_to_client(const PlayerUpdateToClient & pe);
		virtual void player_accepted(const PlayerAccepted & pe);
		virtual void player_rejected(const PlayerRejected & pe);
		virtual void player_send_message(const PlayerSendMessage & pe);
		virtual void player_connected(const PlayerConnected & pe);
		virtual void player_disconnected(const PlayerDisconnected & pe);
		virtual void server_entry_list(const ServerEntryList & pe);
		virtual void load_level(const LoadLevel & pe);
		virtual void create_target(const CreateTarget & pe);
		virtual void score_change(const ScoreChange & pe);
		virtual void destroy_target(const DestroyTarget & pe);

	private:
		Game & game_;
};

#endif // GAMECOMMANDHANDLER_H
