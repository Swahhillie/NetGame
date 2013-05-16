#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <GLM/GLM.hpp>
class KeyState;

class Character
{
	public:
		Character();
		virtual ~Character();

		void makeStep(const KeyState & keys, float moveSpeed, float seconds);

		glm::vec2 getPosition()const{return position_;}
		void setPosition(const glm::vec2 pos){position_ = pos;}

	private:
		glm::vec2 position_;

};

#endif // CHARACTER_HPP
