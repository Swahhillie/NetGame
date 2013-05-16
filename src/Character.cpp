#include "Character.hpp"
#include "FullExpressionAccumulator.hpp"
#include "KeyState.hpp"

Character::Character()
{

}

Character::~Character()
{
    //dtor
}


void Character::makeStep( const KeyState & keys, float moveSpeed, float seconds)
{
    glm::vec2 moveToMake;
    if(keys.up)
    {
        moveToMake.y -= moveSpeed;
    }
    if(keys.down)
    {
        moveToMake.y += moveSpeed;
    }
    if(keys.left)
    {
        moveToMake.x -= moveSpeed;
    }
    if(keys.right)
    {
        moveToMake.x += moveSpeed;
    }

    position_ += moveToMake * seconds;

}
