#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "NetworkInterface.hpp"
#include "Controller.hpp"
#include "Scene.hpp"
#include "NetworkEvents.h"
#include <SFML/Network.hpp>
#include <string>
#include "GameCommandHandler.hpp"
#include "ClientsManager.hpp"
#include "LoginManager.hpp"
class LoginData;

class Game
{
    friend class NetworkInterface; //so it can callback to handle messages
	friend class GameCommandHandler;
	friend class GUI;

public:

	enum GameState{
		Loggin,
		WaitingForAccept,
		Playing
	};

    Game();
    virtual ~Game();


    void build();
    void run();
    void kickOff();
    void onRejectLogin();
    void loadLevel(const std::string & name);
    GameState getGameState()const{return gameState_;}


protected:
private:
    //managers
    sf::RenderWindow window_;
    Controller controller_;
    Scene scene_;
    ClientsManager clientsManager_;

    NetworkInterface networkInterface_;
    GameCommandHandler gameCommandHandler_;
    LoginManager loginManager_;

private:


private:
    //doing stuff

    sf::IpAddress enterIpAddress();
    void setupPlayer(const LoginData & loginData);

    void update();
    void handleMessage(PacketWrap & packetWrap);

    GameState gameState_;
    void setGameState(GameState state);

};

#endif // GAME_H
