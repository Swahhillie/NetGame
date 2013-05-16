#ifndef LOCALPLAYERINFO_H
#define LOCALPLAYERINFO_H

#include <string>
#include <SFML/Graphics.hpp>
#include "NetworkEvents.h"
#include "ControlledCharacter.hpp"
#include "Time.hpp"
class LocalPlayerInfo
{
public:
	friend class ControlledCharacter;
    static LocalPlayerInfo& instance()
    {
        static LocalPlayerInfo INSTANCE;
        return INSTANCE;
    }

    std::string name;
    std::string password;
    sf::Color color;
    sf::IpAddress publicAddress;
    bool isRegistration;
    ControlledCharacter * character; // the players character
	bool shouldSendNewKeyState;
	float serverConnectTime;

    friend PlayerStateUpdate & operator << (PlayerStateUpdate & event, const LocalPlayerInfo & info)
    {
		event.name = info.name;
		//event.sendTime = Time::time + info.serverConnectTime;
		event.keyState = info.character->getKeyState();
		return event;
    }




protected:
private:

    LocalPlayerInfo();
    virtual ~LocalPlayerInfo();
    LocalPlayerInfo(const LocalPlayerInfo& other);
    LocalPlayerInfo& operator=(const LocalPlayerInfo& other);
    void sendNewKeyState(){
		shouldSendNewKeyState = true;
    }

};

#endif // LOCALPLAYERINFO_H
