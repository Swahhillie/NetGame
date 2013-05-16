#include "Controller.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

Controller::Controller(sf::RenderWindow & window):window_(window)
{
    //ctor
    mousePressed = false;
    isRunning_ = true;
    instance = this;
    pressedKeys = std::vector<bool>(sf::Keyboard::KeyCount, false);
    lastKeys = std::vector<bool>(sf::Keyboard::KeyCount, false);
    pressedButtons = std::vector<bool>(sf::Mouse::ButtonCount, false);
    lastButtons = std::vector<bool>(sf::Mouse::ButtonCount, false);

}

Controller::~Controller()
{
    //dtor
}
void Controller::handleEvent(const sf::Event & event)
{


    //capturing text entered events
    if(event.type == sf::Event::TextEntered)
    {
        uint32_t uniC = event.text.unicode;
        if(uniC < 128){
			std::string newTxt = "";
			sf::Utf32::encodeAnsi(uniC, std::back_inserter(newTxt));
			capturedText += newTxt;
        }

    }

	//window close event
    if(event.type == sf::Event::Closed)
    {
        //window_ closed. should destroy everything now..
        isRunning_ = false;
        window_.close();
        return;
    }
//copy values of prev frame in the lastKeys.
    //now the static functions can compare the result to see if there is a difference
    if(event.type == sf::Event::KeyPressed)
    {

       // int keyCodeInt = (int)event.key.code;
        //keyBoolMap::iterator it = pressedKeys.find(keyCodeInt);
		pressedKeys[event.key.code] = true;

    }
    else if(event.type == sf::Event::KeyReleased)
    {

       // int keyCodeInt = (int)event.key.code;
		pressedKeys[event.key.code] = false;

    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        //int buttonCode = (int)event.mouseButton.button;
		pressedButtons[event.mouseButton.button] = true;

    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {

        //int buttonCode = (int)event.mouseButton.button;
		pressedButtons[event.mouseButton.button] = false;

    }



}
void Controller::update()
{

    //if the window_ has been closed the controller will do nothing
    if(!isRunning_)return;


    refresh();

    sf::Event event;
    while(window_.pollEvent(event))
    {
        handleEvent(event);
    }
    mousePosition = glm::vec2(sf::Mouse::getPosition(window_).x, sf::Mouse::getPosition(window_).y);
    //update is used for continuos events suchs when a key is held down.
}
void Controller::refresh()
{
    lastKeys = std::vector<bool>(pressedKeys);

    lastMousePosition = mousePosition;

    lastButtons = std::vector<bool>(pressedButtons);

    capturedText = "";

}

bool Controller::getKey(sf::Keyboard::Key keyCode)
{
    return instance->pressedKeys[keyCode];
}

bool Controller::getKeyUp(sf::Keyboard::Key keyCode)
{
    return instance->lastKeys[keyCode] == true && instance->pressedKeys[keyCode] == false;
}

bool Controller::getKeyDown(sf::Keyboard::Key keyCode)
{
    return instance->lastKeys[keyCode] == false && instance->pressedKeys[keyCode] == true;
}

bool Controller::getButton(sf::Mouse::Button button)
{
    return instance->pressedButtons[button];
}

bool Controller::getButtonUp(sf::Mouse::Button button)
{
    return instance->lastButtons[button] == true && instance->pressedButtons[button] == false;
}

bool Controller::getButtonDown(sf::Mouse::Button button)
{
    return instance->lastButtons[button] == false && instance->pressedButtons[button] == true;
}

glm::vec2 Controller::getDeltaMouse()
{
    return lastMousePosition - mousePosition;
}

bool Controller::getTextEntered(std::string & input)
{
    if(instance->capturedText != "")
    {
        input = instance->capturedText;
        return true;
    }
    else return false;
}
Controller * Controller::instance;
glm::vec2 Controller::mousePosition;
glm::vec2 Controller::lastMousePosition;
