#ifndef CHARACTERGAMEOBJECT_H
#define CHARACTERGAMEOBJECT_H

#include "GameObject.hpp"
#include "Character.hpp"

class CharacterGameObject : public GameObject, public Character
{
	public:
		CharacterGameObject(const std::string & name, const sf::Color & color);
		virtual ~CharacterGameObject();


	protected:

		virtual void update();
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;

		float playerMoveSpeed;
	private:

		sf::CircleShape circle;
	private:
		void setupCircle(const sf::Color & c);


};

#endif // CHARACTERGAMEOBJECT_H
