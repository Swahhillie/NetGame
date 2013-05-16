#include "ControlledCharacter.hpp"
#include "FullExpressionAccumulator.hpp"
#include "Time.hpp"
#include "Controller.hpp"
#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>
#include "Transform.hpp"
#include "LocalPlayerInfo.hpp"
#include "ConfigLoader.hpp"
#include "GUI.hpp"
#include <sstream>



ControlledCharacter::ControlledCharacter(const std::string & name, const sf::Color& color)
    :NetworkedCharacter(name, color)
{
    //ctor

    LocalPlayerInfo::instance().character = this;

}


ControlledCharacter::~ControlledCharacter()
{
    //dtor
}

void ControlledCharacter::start()
{
    FullExpressionAccumulator(std::cout) << "Started " << name << "\n";
}

void ControlledCharacter::update()
{

    storeKeyState();
	//makeStep(keyState, playerMoveSpeed, Time::deltaTime);
	NetworkedCharacter::update();

}

void ControlledCharacter::storeKeyState(){
	keyState.up = Controller::getKey(sf::Keyboard::W);
	keyState.down = Controller::getKey(sf::Keyboard::S);
	keyState.left = Controller::getKey(sf::Keyboard::A);
	keyState.right = Controller::getKey(sf::Keyboard::D);

	if(keyState != lastKeyState_){ //if keystates are not the same
		LocalPlayerInfo::instance().sendNewKeyState();//send a keystate change event to the serve
	}


	lastKeyState_ = keyState;

}


