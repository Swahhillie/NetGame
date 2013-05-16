#ifndef TIME_H
#define TIME_H

#include <SFML/System.hpp>

class Game;
class Server;

class Time
{
public:

    virtual ~Time();

    static float deltaTime;
    static float time;

protected:
private:

    static sf::Clock clock;

    static void update()
    {
        float newTime = clock.getElapsedTime().asSeconds();
        deltaTime_ = newTime - time_;
        time_ = newTime;
        deltaTime = deltaTime_;
        time = time_;
    }


    static float deltaTime_;
    static float time_;



    Time();
    Time(const Time& other);
    Time& operator=(const Time& other);

    friend class Game;
    friend class Server;
};

#endif // TIME_H
