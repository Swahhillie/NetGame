#include "GameCommandHandler.hpp"
#include "Game.hpp"
#include "LocalPlayerInfo.hpp"

GameCommandHandler::GameCommandHandler(Game & game):game_(game)
{
    //ctor
}

GameCommandHandler::~GameCommandHandler()
{
    //dtor
}
void GameCommandHandler::player_update_to_client(const PlayerUpdateToClient & pe)
{

    if(game_.gameState_ == Game::Playing)
    {
        static float lastUpdate = 0;
        if(lastUpdate < pe.serverTime)
        {
            for(unsigned int i = 0 ; i < pe.entryCount; i ++)
            {

                const PlayerStateUpdate & p = pe.players[i];

                p.receiveTime = Time::time;/*
                if(p.name != LocalPlayerInfo::instance().name)
                {
                */
                    game_.clientsManager_.updateClient(p);
/*
                }

                else
                {


                    LocalPlayerInfo::instance().character->setKeyState( p.keyState );
                    LocalPlayerInfo::instance().character->setPosition( p.position );

                }

                */
            }
        }
        else
        {
            if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "A more recent update was already received \n";
        }
    }

}
void GameCommandHandler::player_accepted(const PlayerAccepted & pe)
{

    LocalPlayerInfo::instance().serverConnectTime = pe.serverTime;
    game_.kickOff();
}
void GameCommandHandler::player_rejected(const PlayerRejected & pe)
{
    game_.onRejectLogin();
    game_.loginManager_.setInfo(std::string("Rejected because: ") + pe.reason);
}
void GameCommandHandler::player_send_message(const PlayerSendMessage & pe)
{

}
void GameCommandHandler::player_connected(const PlayerConnected & pe)
{

    std::cout << "game command handler received a player connected event for player : " << pe.name << std::endl;

    if(game_.clientsManager_.addClient(pe.name, pe.color))
    {

        if(pe.name == LocalPlayerInfo::instance().name)
        {
            std::cout << " Received a player_connected signal for myself" << std::endl;
            game_.clientsManager_.bindCharacterToClient(pe.name, LocalPlayerInfo::instance().character);
        }
        else
        {
            NetworkedCharacter * networkedChar = game_.scene_.createNetworkedCharacter(pe.name, pe.color);
            game_.clientsManager_.bindCharacterToClient(pe.name, networkedChar);

        }

    }
    else
    {
        assert(1 == 2); // somehow there are two idenctically named clients on this server.
    }
}
void GameCommandHandler::player_disconnected(const PlayerDisconnected & pe)
{
    std::cout << "removed " << pe.name << " from the game, he has disconnected " << std::endl;
    game_.scene_.removeNetworkedCharacter(pe.name);
    game_.clientsManager_.removeClient(pe.name);
}
void GameCommandHandler::server_entry_list(const ServerEntryList & pe)
{
    if(pe.entryCount > 0)FullExpressionAccumulator(std::cout) << pe.entries[0].ip.toString() << "\n";
    FullExpressionAccumulator(std::cout) << pe.getFormattedSting();
}
void GameCommandHandler::load_level(const LoadLevel & pe)
{
    game_.loadLevel(pe.levelName);

}
void GameCommandHandler::create_target(const CreateTarget & pe)
{
    game_.scene_.createTarget(pe.targetId, pe.position);
}
void GameCommandHandler::score_change(const ScoreChange & pe)
{
    for(unsigned int i = 0; i < pe.entryCount; i++)
        game_.clientsManager_.setScore(pe.scoreObjects[i].name, pe.scoreObjects[i].score);
}
void GameCommandHandler::destroy_target(const DestroyTarget & pe)
{
    game_.scene_.removeTarget(pe.targetId);
}
