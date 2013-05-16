#include "CommandHandler.hpp"
#include "FullExpressionAccumulator.hpp"

CommandHandler::CommandHandler()
{
    //ctor
}

CommandHandler::~CommandHandler()
{
    //dtor
}

void CommandHandler::handlePacket(PacketWrap & packwrap)
{
    sf::Int32 command = 0;

    sf::Packet packet = packwrap.packet;

    if(packet >> command)
    {

        if(Debug::VERBOSE_MESSAGES && !isSilent(command))FullExpressionAccumulator(std::cout) << "Received a " << Network::commandToString(command) << " command \n";
        switch(command)
        {
        case NetworkEvent::PLAYER_CONNECTED:
        {
            PlayerConnected pe;
            packet >> pe;
            player_connected(pe);
            break;
        }
        case NetworkEvent::PLAYER_AUTHORIZE:
        {
            PlayerAuthorize pe;
            packet >> pe;
            player_authorize(pe, packwrap.fromIp, packwrap.fromPort);
            break;
        }

        case NetworkEvent::PLAYER_DISCONNECTED:
        {
            PlayerDisconnected pe;
            packet >> pe;
            player_disconnected(pe);
            break;
        }
        case NetworkEvent::PLAYER_SEND_MESSAGE:
        {
            PlayerSendMessage pe;
            packet >> pe;
            player_send_message(pe);
            break;
        }
        case NetworkEvent::PLAYER_REJECTED:
        {
            PlayerRejected pe;
            packet >> pe;
            player_rejected(pe);
            break;
        }
        case NetworkEvent::PLAYER_ACCEPTED:
        {
            PlayerAccepted pe;
            packet >> pe;
            player_accepted(pe);
            break;
        }
        case NetworkEvent::PLAYER_UPDATE_TO_SERVER:
        {
            PlayerStateUpdate pe;
            packet >> pe;
            player_update_to_server(pe);
            break;
        }
        case NetworkEvent::PLAYER_UPDATE_TO_CLIENT:
        {
            PlayerUpdateToClient pe;
            packet >> pe;
            player_update_to_client(pe);
            break;
        }
        case NetworkEvent::SERVER_ENTRY_GET:
        {
            ServerEntryGet pe;
            packet >> pe;
            server_entry_get(pe);
            break;
        }
        case NetworkEvent::SERVER_ENTRY_ADD:
        {
            ServerEntryAdd pe;
            packet >> pe;
            server_entry_add(pe);
            break;
        }
        case NetworkEvent::SERVER_ENTRY_LIST:
        {
            ServerEntryList pe;
            packet >> pe;
            server_entry_list(pe);
            break;
        }
        case NetworkEvent::SERVER_ENTRY_ACCEPT:
        {
            ServerEntryAccept pe;
            packet >> pe;
            server_entry_accept(pe);
            break;
        }
        case NetworkEvent::SERVER_ENTRY_REJECT:
        {
            ServerEntryReject pe;
            packet >> pe;
            server_entry_reject(pe);
            break;
        }
        case NetworkEvent::LOAD_LEVEL:
        {
            LoadLevel pe;
            packet >> pe;
            load_level(pe);
            break;
        }
        case NetworkEvent::CREATE_TARGET:
        {
            CreateTarget pe;
            packet >> pe;
            create_target(pe);
            break;
        }
        case NetworkEvent::DESTROY_TARGET:
        {
            DestroyTarget pe;
            packet >> pe;
            destroy_target(pe);
            break;
        }
        case NetworkEvent::SCORE_CHANGE:
        {
            ScoreChange pe;
            packet >> pe;
            score_change(pe);
            break;
        }
        //target list gets split into multiple single targets
        case NetworkEvent::TARGET_LIST:
        {
            TargetList tl;
            packet >> tl;
            for(unsigned int i = 0; i < tl.entryCount; i++)
            {
                CreateTarget pe;
                pe.targetId = tl.targetIds[i];
                pe.position = tl.targetPositions[i];
                create_target(pe);

            }
            break;
        }

        }


    }
    else
    {
        std::string debugStr;
        packet >> debugStr;
        FullExpressionAccumulator(std::cout) << "Packet did not contain a valid command to start with\nIn string form it was : '" << debugStr << "'\n";
    }

}
bool CommandHandler::isSilent(sf::Int32 command)
{
    sf::Int32 silentCommands [] = { NetworkEvent::PLAYER_UPDATE_TO_CLIENT};
    unsigned int silentCommandsCount = sizeof(silentCommands) / sizeof(sf::Int32); //calculate size of silent commands to keep it dynamic
    bool isSilent = false;
    for(unsigned int i = 0; i < silentCommandsCount; i++)
    {
        if(silentCommands[i] == command)
        {
            isSilent = true;
            break;
        }
    }
    return isSilent;
}
/*
onPlayerConnected()
player_disconnected()
player_send_message()
player_rejected()
player_accepted()
player_update_to_server())
player_update_to_client()
server_entry_get()
server_entry_add()
server_entry_list()
server_entry_accept()
server_entry_reject()
*/
