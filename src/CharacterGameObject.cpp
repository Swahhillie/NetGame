#include "CharacterGameObject.hpp"
#include "ConfigLoader.hpp"
#include "FullExpressionAccumulator.hpp"
#include "Transform.hpp"
#include  <math.h>
CharacterGameObject::CharacterGameObject(const std::string & name, const sf::Color & color):
    GameObject(name)
{
    playerMoveSpeed = ConfigLoader::instance().getFloat("settings,playerCharacter,moveSpeed");
    setupCircle(color);
}


CharacterGameObject::~CharacterGameObject()
{
	//dtor
}
void CharacterGameObject::draw(sf::RenderTarget& target, sf::RenderStates states)const
{

    //std::cout << "pos  = " << transform.getPosition().x << ", " << transform.getPosition().y << std::endl << "vel = " << velocity.x << " , " <<  velocity.y;

    target.draw(circle,states);
    //FullExpressionAccumulator(std::cout) << "Drawing Character\n";
}
void CharacterGameObject::setupCircle(const sf::Color & color)
{
	float radius = 10;
    circle.setRadius(radius);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(5);
    circle.setOrigin(radius, radius);

}
void CharacterGameObject::update(){
	transform.setLocalPosition(getPosition());
    circle.setPosition(transform.getLocalPosition().x, transform.getLocalPosition().y);
}
