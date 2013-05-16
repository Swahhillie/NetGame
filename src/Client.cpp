#include "Client.hpp"
#include "Character.hpp"
#include "ConfigLoader.hpp"

Client::Client(const std::string & aName, const sf::Color & aColor, const sf::IpAddress & aIp):
name(aName),
color(aColor),
ip(aIp),
score(0),
character(NULL)
{
    //ctor
    moveSpeed = ConfigLoader::instance().getFloat("settings,playerCharacter,moveSpeed");
}

Client::~Client()
{
    //dtor
}

