#include "LocalPlayerInfo.hpp"

LocalPlayerInfo::LocalPlayerInfo():character(NULL)
{
    //ctor
    publicAddress = sf::IpAddress::getPublicAddress(sf::seconds(2.0f));
    shouldSendNewKeyState = false;
}

LocalPlayerInfo::~LocalPlayerInfo()
{
    //dtor
}

LocalPlayerInfo::LocalPlayerInfo(const LocalPlayerInfo& other)
{
    //copy ctor
}

LocalPlayerInfo& LocalPlayerInfo::operator=(const LocalPlayerInfo& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}


