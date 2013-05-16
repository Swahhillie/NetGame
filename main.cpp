#include <iostream>

#include "Server.hpp"

#include "Game.hpp"
#include "MasterServer.hpp"

#include "DebugSpace.h"
#include "FullExpressionAccumulator.hpp"
#include "ConfigLoader.hpp"
#include <sstream>

void setSettings(){
	ConfigLoader & configLoader = ConfigLoader::instance();
	Debug::DISCOVER_SERVERS = 			configLoader.getBoolSetting("discoverServers");
	Debug::VERBOSE_MESSAGES = 			configLoader.getBoolSetting("verboseMessages");
	Debug::SKIP_PLAYER_COLOR = 			configLoader.getBoolSetting("skipPlayerColor");
	Debug::SKIP_PLAYER_NAME = 			configLoader.getBoolSetting("skipPlayerName");
	Debug::SKIP_IP = 					configLoader.getBoolSetting("skipIp");
	Debug::AUTOFILL_LOGGIN = 			configLoader.getBoolSetting("autoFillLogin");
	Debug::REQUIRE_SERVER_CONNECTION = 	configLoader.getBoolSetting("requireServerConnection");
}
std::string placeTextInBlock(std::string txt){
	int barCount = 40;
	int textCount = txt.size();
	std::stringstream stream;
	for(int i = 0 ; i < barCount; i++)
		stream << "-";

	stream << std::endl;
	for(int i = 0; i < (barCount - textCount) / 2; i++)
		stream << "-";
	stream << txt;

	for(int i = 0; i < (barCount - textCount) / 2; i++)
		stream << "-";

	stream << std::endl;
	for(int i = 0 ; i < barCount; i++)
		stream << "-";

	stream<< std::endl;

	return stream.str();
}
int main()
{
    std::cout << "-------Networking game by David Hoogenbosch-------" << std::endl;
    setSettings();

    std::cout << "Last Compiled on " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "Verbose Messages == " << Debug::VERBOSE_MESSAGES << std::endl;
    std::cout << "SkipEnterPlayerColor == " << Debug::SKIP_PLAYER_COLOR << std::endl;
    std::cout << "SkipEnterPlayerName == " << Debug::SKIP_PLAYER_NAME << std::endl;
    std::cout << "SkipEnterIp == "<< Debug::SKIP_IP << std::endl;
    std::cout << "RequireSeverConnection == "<< Debug::REQUIRE_SERVER_CONNECTION << std::endl;
    std::cout << "discoverServers == "<< Debug::DISCOVER_SERVERS<< std::endl;
    std::cout << "autoFillLogin == "<< Debug::AUTOFILL_LOGGIN<< std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    static const int RUNSERVER = 1;
    static const int RUNGAME = 2;
    static const int RUNMASTERSERVER = 3;

    std::cout << "type: \"1\" to run server" << std::endl;
    std::cout << "type: \"2\" to run game" << std::endl;
    std::cout << "type: \"3\" to run master server" << std::endl;

    int input = 0;
    std::cin >> input;

    if(input == RUNSERVER)
    {
	FullExpressionAccumulator(std::cout) << placeTextInBlock("SERVER START");
        Server server;
        server.build();
        server.run();
        FullExpressionAccumulator(std::cout) << placeTextInBlock("SERVER END");
    }
    else if (input == RUNGAME)
    {
        FullExpressionAccumulator(std::cout) << placeTextInBlock("RUNNING GAME");
        Game game;
        game.build();
        game.run();
        FullExpressionAccumulator(std::cout) << placeTextInBlock("GAME END");

    }
    else if(input == RUNMASTERSERVER)
    {
		FullExpressionAccumulator(std::cout) << placeTextInBlock("RUNNING MASTER SERVER");
		MasterServer masterServer;
		masterServer.build();
		masterServer.run();
		FullExpressionAccumulator(std::cout) << placeTextInBlock("ENDING MASTER SERVER");
    }
    else{
        std::cout << "didn't recognise" << std::endl;
    }



    return 0;
}
