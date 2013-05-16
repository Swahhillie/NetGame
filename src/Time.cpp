#include "Time.hpp"

Time::Time()


{
    //ctor
}

Time::~Time()
{
    //dtor
}

Time::Time(const Time& other)

{
    //copy ctor
}

Time& Time::operator=(const Time& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}


float Time::time;
float Time::time_;
float Time::deltaTime;
float Time::deltaTime_;
sf::Clock Time::clock;
