#ifndef NETWORKEDCHARACTER_H
#define NETWORKEDCHARACTER_H

#include "CharacterGameObject.hpp"
#include "KeyState.hpp"


class NetworkedCharacter : public CharacterGameObject
{
	public:
		NetworkedCharacter(const std::string &, const sf::Color &);
		virtual ~NetworkedCharacter();

		KeyState keyState;
	protected:
		virtual void update();
	private:

};

#endif // NETWORKEDCHARACTER_H
