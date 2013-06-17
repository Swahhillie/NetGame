#include "TextInputField.hpp"
#include <assert.h>
#include "Controller.hpp"
#include <iostream>
#include "GUI.hpp"

TextInputField::TextInputField(const std::string & name, const sf::Vector2f & position, const sf::Vector2f & aSize):
    GameObject(name),
    isOpen_(false),
    topLeft(position.x, position.y),
    size(aSize.x, aSize.y)

{
    //ctor
    const sf::Font & font = GUI::instance->getFont();
    isReady_ = false;
    isOpen_ = false;
    labelHeight = 20;
    label_.setPosition(position.x,position.y-20);
    label_.setCharacterSize(16);
    label_.setString("");
    label_.setColor(sf::Color::Black);
    txt_.setFont(font);
    txt_.setPosition(position);
    txt_.setColor(sf::Color::Black);
    txt_.setCharacterSize(20);
    box_.setPosition(position);
    box_.setSize(size);
    box_.setFillColor(sf::Color::Transparent);
    box_.setOutlineColor(sf::Color::Black);
    box_.setOutlineThickness(3.0f);
    allowedLetters_ = "0123456789.,_/\[]{}!@#$%^&*()abcdefghijklmnopqrstuvwxyz ";
    isReady_ = false;
}

TextInputField::~TextInputField()
{
    //dtor
}
void TextInputField::setAllowedLetters(const std::string & str)
{
    allowedLetters_ = str;
}
void TextInputField::open()
{
    assert(isOpen_ == false);
    isOpen_ = true;
    box_.setOutlineColor(sf::Color::Red);
    std::cout << "Opened text field" << std::endl;
}
void TextInputField::close()
{
    assert(isOpen_);
    isOpen_ = false;
    box_.setOutlineColor(sf::Color::Black);
    std::cout << "Closed text field" << std::endl;
}
void TextInputField::update()
{

    detectClick();

    if(isOpen_)
    {
        std::string input = "";

        if(Controller::getTextEntered(input))
        {
            if(allowedLetters_.find(std::tolower(input[0])) != std::string::npos)
            {
                currentText_ += input;
            }

        }

        if(Controller::getKeyDown(sf::Keyboard::Key::Return))
        {
            close();
            isReady_ = true;
        }

        if(Controller::getKeyDown(sf::Keyboard::Key::BackSpace))
        {
            currentText_ = currentText_.substr(0, currentText_.size() -1);
        }
        txt_.setString(currentText_);
    }
}

void TextInputField::detectClick()
{


    if(Controller::getButtonDown(sf::Mouse::Left))
    {

        glm::vec2 clickPos = Controller::mousePosition;

        if( clickPos.x > topLeft.x && clickPos.x < topLeft.x + size.x && clickPos.y > topLeft.y && clickPos.y < topLeft.y + size.y)
            {
                //clicked on the rect
                if(isOpen_ == false) open();

            }
            else
            {
                //clicked outside of the box
                if(isOpen_ == true) close();
            }

    }

}
void TextInputField::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
    target.draw(box_);
    target.draw(label_);
    target.draw(txt_);
}
void TextInputField::setLabelText(const std::string & labelText)
{
    label_.setString(labelText);
}
