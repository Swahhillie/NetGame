#include "TargetBlock.hpp"
#include "Time.hpp"
#include <math.h>
#include "Transform.hpp"

TargetBlock::TargetBlock(const std::string & name,const sf::Texture & t):
	GameObject(name)
{
	//ctor
	sprite.setTexture(t);
	sprite.setOrigin(8,8);

}

TargetBlock::~TargetBlock()
{
	//dtor
}
void TargetBlock::update(){
	float rotationSpeed = 1;
	float angle = (Time::time - (int)(Time::time)) * rotationSpeed * 360;
	sprite.setRotation(angle);
	sprite.setPosition(transform.getPositionSf());
}
void TargetBlock::draw(sf::RenderTarget & target, sf::RenderStates states)const{
	target.draw(sprite);
}
