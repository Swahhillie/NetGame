#include "NetworkedCharacter.hpp"
#include "Time.hpp"
#include <iostream>

NetworkedCharacter::NetworkedCharacter(const std::string & name,const sf::Color & color):
	CharacterGameObject(name, color)
{
	//ctor

}

NetworkedCharacter::~NetworkedCharacter()
{
	//dtor
}
void NetworkedCharacter::update(){
	makeStep(keyState, playerMoveSpeed, Time::deltaTime);
	CharacterGameObject::update();

}
