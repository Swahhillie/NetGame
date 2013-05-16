#ifndef CONTROLLED_CHARACTER_H
#define CONTROLLED_CHARACTER_H

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "NetworkedCharacter.hpp"
#include "KeyState.hpp"
#include "Character.hpp"
class ControlledCharacter : public NetworkedCharacter
{
public:
    explicit ControlledCharacter(const std::string & name, const sf::Color & color);

    virtual ~ControlledCharacter();

    KeyState getKeyState()const{return keyState;}
    void setKeyState(const KeyState & a_keyState){keyState = a_keyState;}

protected:

    virtual void start();
    virtual void update();


private:



private:

	KeyState lastKeyState_;

	 void storeKeyState();
};

#endif // CONTROLLED_CHARACTER_H
