#include "GameObject.hpp"
#include <assert.h>
#include "Transform.hpp"
#include <iostream>


GameObject::GameObject(const std::string & name)
    :transform(*(new Transform(*this)))
{
    this->name = name;
    isMarkedForDestroy = false;
}

GameObject::~GameObject()
{
    //dtor
	std::cout << "------------deleting " << name << std::endl;
    delete &transform;
}

void GameObject::ghostUpdate()
{
    Transform::Children & children = transform.getChildren();
    for(int i = children.size() -1; i >= 0; i--)
    {
        Transform & t = *children[i];
        GameObject & go = t.gameObject;
        if(go.started_ == false)
        {
            go.started_ = true;
            go.start();
        }
        go.ghostUpdate();
    }
    update(); // execute behaviour
}
void GameObject::ghostDraw(sf::RenderTarget & target, sf::RenderStates states)const
{
    Transform::Children & children = transform.getChildren();
    for(int i = children.size() -1; i >= 0; i--)
    {
        Transform & t = *children[i];
        GameObject & go = t.gameObject;
        target.draw(go);
    }
    target.draw(*this);
}
void GameObject::cleanup()
{
    Transform::Children & children = transform.getChildren();
    for(int i = children.size() -1; i>= 0 ; i--)
    {
        Transform & t = *children[i];
        GameObject & go = t.gameObject;
        go.cleanup();
    }
    if(isMarkedForDestroy)delete this;
}
void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states)const{

}
void GameObject::onDestroy(){
	std::cout << "------ on destroy " << name << std::endl;
}
void GameObject::destroy(GameObject & go){

	Transform::Children & children = go.transform.getChildren();
    for(int i = children.size() -1; i >= 0; i--)
    {
        Transform & t = *children[i];
        GameObject & go = t.gameObject;
        destroy(go);
    }
	go.onDestroy();
    go.isMarkedForDestroy = true;
}
