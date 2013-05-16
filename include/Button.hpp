#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "GameObject.hpp"

#include <SFML/Graphics.hpp>

class Button : public GameObject
{
	public:
		Button(const std::string & name, const sf::Vector2f & position, const sf::Vector2f & size);
		virtual ~Button();

		void setText(const std::string & txt);
		bool wasClicked()const{return wasClicked_;}

	protected:
		virtual void update();
		virtual void draw(sf::RenderTarget & target, sf::RenderStates renderStates)const;
	private:
		bool wasClicked_; // true if button was clicked in the last frame
		sf::Vector2f position_;
		sf::Vector2f size_;


		sf::RectangleShape box_;
		sf::Text txt_;

};

#endif // BUTTON_HPP
