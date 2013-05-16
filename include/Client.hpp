#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "KeyState.hpp"
#include <glm/glm.hpp>
#include "NetworkEvents.h"
#include <queue>
class Character;

class Client
{
public:
    Client(const std::string & name, const sf::Color & color, const sf::IpAddress & ip);
    virtual ~Client();

    std::string name;
    sf::Color color;
    sf::IpAddress ip;
    unsigned short port;
    KeyState keyState;
    glm::vec2 position;
    float moveSpeed;
    int score;
    Character * character; // used by the game client managager to move character in the world

    float time; // the moment up to wich this client is simulated on the server

    std::queue<PlayerStateUpdate> toProcessUpdates;

};

#endif // CLIENT_H
