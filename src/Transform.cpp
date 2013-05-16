#include "Transform.hpp"
#include "GameObject.hpp"
#include <assert.h>
#include "FullExpressionAccumulator.hpp"
#include <iostream>

Transform::Transform(GameObject & aGameObject):
    gameObject(aGameObject),
    parent_(NULL)
{
    //ctor
}

Transform::~Transform()
{
    //dtor
    assert(gameObject.isMarkedForDestroy); //call delete on the gameobject, not on the transform
    setParent(NULL);
}
//glm::vec2 Transform::getPosition()const
//{
//    return parent_? parent_->getPosition()+localPosition_ : localPosition_;
//}
glm::vec2 Transform::getLocalPosition()const
{
    return localPosition_;
}
//void Transform::setPosition(const glm::vec2 & newPos)
//{
//    //set position in world space
//    localPosition_ = parent_? parent_->getPosition() - newPos : newPos;
//}
void Transform::setLocalPosition(const glm::vec2 & newPos)
{
    localPosition_ = newPos;
}
void Transform::addChild(Transform * child)
{
    children_.push_back(child);
}
void Transform::removeChild(Transform * child)
{
    for(int i = children_.size() -1; i >= 0; i--)
		if(children_[i] == child)children_.erase(children_.begin() + i);
}
void Transform::setParent(Transform * newParent)
{

    if(parent_)
    {
        parent_->removeChild(this);
    }
    if(newParent)
    {
        newParent->addChild(this);
    }
    else
    {
        FullExpressionAccumulator(std::cout) << "This object no longer has a parent, it should be destroyed\n";
    }

    parent_ = newParent;
}
