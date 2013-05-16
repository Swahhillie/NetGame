#include "Mutexes.hpp"

Mutexes::Mutexes()
{
    //ctor
}

Mutexes::~Mutexes()
{
    //dtor
}

Mutexes::Mutexes(const Mutexes& other)
{
    //copy ctor
}

Mutexes& Mutexes::operator=(const Mutexes& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
sf::Mutex Mutexes::stdcout;
sf::Mutex Mutexes::messages;
