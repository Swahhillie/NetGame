#include "Button.hpp"
#include "GUI.hpp"
#include "Controller.hpp"


Button::Button(const std::string & name, const sf::Vector2f & position, const sf::Vector2f & size):
	GameObject(name),
	position_(position),
	size_(size)
{
    //ctor

    txt_.setFont(GUI::instance->getFont());
    txt_.setPosition(position);
    txt_.setColor(sf::Color::Black);
    txt_.setCharacterSize(20);

    box_.setPosition(position);
    box_.setSize(size);
    box_.setFillColor(sf::Color::Transparent);
    box_.setOutlineColor(sf::Color::Black);
    box_.setOutlineThickness(3.0f);

    wasClicked_ = false;


}

Button::~Button()
{
    //dtor
}
void Button::update()
{
	wasClicked_ = false;
	if(Controller::getButtonDown(sf::Mouse::Left)){
		glm::vec2 mouseClick = Controller::mousePosition;
		if(mouseClick.x > position_.x && mouseClick.x < position_.x + size_.x && mouseClick.y > position_.y && mouseClick.y < position_.y + size_.y){
			wasClicked_ = true;
		}
	}

}
void Button::draw(sf::RenderTarget & target, sf::RenderStates renderStates)const
{
	target.draw(txt_);
}
void Button::setText(const std::string & txt){
	txt_.setString(txt);
}
