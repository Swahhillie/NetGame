#ifndef TARGETBLOCK_H
#define TARGETBLOCK_H

#include "GameObject.hpp"

#include <SFML/Graphics.hpp>

class TargetBlock : public GameObject
{
	public:
		TargetBlock(const std::string & name, const sf::Texture& t);
		virtual ~TargetBlock();
	protected:

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
		virtual void update();
	private:

		sf::Sprite sprite;
};

#endif // TARGETBLOCK_H
