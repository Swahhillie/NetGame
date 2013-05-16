#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include "FullExpressionAccumulator.hpp"
#include "LocalPlayerInfo.hpp"
#include "Time.hpp"
#include "DebugSpace.h"
#include "NetworkEvents.h"
#include  "ConfigLoader.hpp"
#include <vector>
#include "GUI.hpp"

Game::Game():
    window_(sf::VideoMode(1024, 562), "NetGameSimple"),
    controller_(window_),
    scene_(window_, *this),
    gameCommandHandler_(*this),
    loginManager_(scene_)
{
    //ctor
    //giving access to the game class
    GUI::instance->setGame(this);

    ConfigLoader & config = ConfigLoader::instance();

    std::vector<int> res = config.getIntArray("settings,resolution");
    int frameRate  =config.getInt("settings,frameRateLimit");
    assert(res.size() == 2);
    window_.setSize(sf::Vector2u(res[0], res[1]));
    window_.setFramerateLimit(frameRate);
    window_.setVerticalSyncEnabled(true);
    gameState_ = Loggin;

}

Game::~Game()
{
    //dtor
}
//-----------------------------PUBLIC FUNCTIONS
void Game::build()
{
    loginManager_.startLogin();

}
void Game::loadLevel(const std::string & name){
	FullExpressionAccumulator(std::cout) << "LOADING LEVEL " << name << "\n";
	scene_.loadLevel(name);
	clientsManager_.setActiveLevel(scene_.getLevel());
}
void Game::setupPlayer(const LoginData & loginData)
{


    LocalPlayerInfo & playerInfo = LocalPlayerInfo::instance();
    playerInfo.name = loginData.name;
    playerInfo.password = loginData.password;
    playerInfo.isRegistration = loginData.isRegistration;
    playerInfo.color = sf::Color::Blue;

    sf::IpAddress inputIpAddress;
    if(Debug::SKIP_IP)
    {
        sf::IpAddress defaultServerAddress = sf::IpAddress(ConfigLoader::instance().getString("settings,defaultServerAddress"));
        inputIpAddress = defaultServerAddress;
        FullExpressionAccumulator(std::cout) << "Skipping enter ip, Connecting to default address (set in config)\n";
    }
    else
    {
        inputIpAddress = sf::IpAddress(loginData.ipAddress);
        FullExpressionAccumulator(std::cout) << "Entered ip, Connecting to " << inputIpAddress.toString() << "\n";
    }
    if(networkInterface_.getConnected() == false)
    {
        if(networkInterface_.connectToIp(inputIpAddress))
        {
        	networkInterface_.sendAuthorizeMessage();
        }


        else
        {
            FullExpressionAccumulator(std::cout) << "Failed to connect to ip, PLAYING LOCALLY " << inputIpAddress.toString();
			kickOff();
            if(Debug::REQUIRE_SERVER_CONNECTION)
            {
                FullExpressionAccumulator(std::cout) << ", closing game\n";
                controller_.stopRunning();
            }
        }
    }
    else{
		networkInterface_.sendAuthorizeMessage();
    }



    /* if(Debug::SKIP_PLAYER_NAME)
     {


         FullExpressionAccumulator(std::cout) << "Skipping loggin, ";
         playerInfo.name = "David";

     }
     else
     {

         FullExpressionAccumulator(std::cout) << "What is your name?\n";
         std::cin >> playerInfo.name;

     }
     if(Debug::SKIP_PLAYER_COLOR)
     {
         playerInfo.color = sf::Color::Red;
     }
     else
     {
         int r = 0;
         int g = 0;
         int b = 0;

         FullExpressionAccumulator(std::cout) << "What is your color?\nr = ";
         std::cin >> r;
         FullExpressionAccumulator(std::cout) << "g = ";
         std::cin >> g;
         FullExpressionAccumulator(std::cout) << "b = ";
         std::cin >> b;

         //negative values dont work...
         playerInfo.color = sf:scene_(scene),:Color(((r<0? 1: r) > 255? 255: r),
                                      ((g<0? 1: g) > 255? 255: g),
                                      ((b<0? 1: b) > 255? 255: b));
         FullExpressionAccumulator(std::cout) << "\n";
     }

     FullExpressionAccumulator(std::cout) << "You are \"" << playerInfo.name << "\", with color: " << playerInfo.color << "\n";
     */
}
void Game::onRejectLogin()
{
    setGameState(Loggin);
    loginManager_.startLogin();
}
void Game::kickOff()
{
    assert(gameState_ == WaitingForAccept || networkInterface_.getConnected() == false);
    std::cout << "START PLAYING" << std::endl;
    setGameState(Playing);
    scene_.createCharacter();
}
void Game::setGameState(Game::GameState state)
{
    std::cout << "ENTERTING GAMESTATE ";
    if(state == Loggin) std::cout << "LOGGIN";
    if(state == WaitingForAccept) std::cout << "WAITING_FOR_ACCEPT";
    if(state == Playing) std::cout << "PLAYING";
    std::cout << std::endl;
    gameState_ = state;
}

sf::IpAddress Game::enterIpAddress()
{
    FullExpressionAccumulator(std::cout) << "Enter server ip address: xxxx.xxxx.xxxx.xxx\n";
    std::string ipString;
    std::cin >> ipString;
    return sf::IpAddress(ipString);
}
void Game::run()
{
    while(controller_.running())
    {
        Time::update();
        this->update();
        switch(gameState_)
        {
        case Loggin:
            loginManager_.update();
            if(loginManager_.isReady())
            {
                setupPlayer(loginManager_.getLoginData());
                if(networkInterface_.getConnected())setGameState(WaitingForAccept);
            }
            break;
        case WaitingForAccept:
            //wait for server accept message, should break out of this at some point
            break;
        case Playing:
            clientsManager_.extrapolatePositions();
            networkInterface_.sendStateUpdate();
            break;
        }

        //FullExpressionAccumulator(std::cout) << "end of frame\n";
    }
    networkInterface_.disconnectFromServer();
}
//-----------------------------PRIVATE FUNCTIONS

void Game::update()
{
    networkInterface_.handleMessages(*this);
    controller_.update();
    scene_.update();
    scene_.draw();
}

void Game::handleMessage(PacketWrap & packetWrap)
{

    gameCommandHandler_.handlePacket(packetWrap);

}
