#ifndef MUTEXES_H
#define MUTEXES_H

#include <SFML/System.hpp>

class Mutexes
{
public:

    virtual ~Mutexes();

    static sf::Mutex stdcout; // mutex for locking access to cout
    static sf::Mutex messages; // mutex for locking access to the message queue

protected:
private:
    Mutexes();
    Mutexes(const Mutexes& other);
    Mutexes& operator=(const Mutexes& other);
};

#endif // MUTEXES_H
