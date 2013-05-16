#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <vector>
#include <SFML/Graphics.hpp>

class GameObject;

class Transform
{
    friend class GameObject;
public:

    virtual ~Transform();

    typedef std::vector<Transform*> Children;

    GameObject & gameObject;

    //glm::vec2 getPosition()const;
    glm::vec2 getLocalPosition()const;

	sf::Vector2f getPositionSf()const{return sf::Vector2f(localPosition_.x, localPosition_.y);}
    //void setPosition(const glm::vec2 & newPos);
    void setLocalPosition(const glm::vec2 & newPos);

    void setParent(Transform * par);
    Transform & getParent()const
    {
        return *parent_;
    }
    Children & getChildren()
    {
        return children_;
    }

protected:
private:
    Transform(GameObject & gameObject);
    Transform * parent_;

    void addChild(Transform * child);
    void removeChild(Transform * child);


    Children children_;

    glm::vec2 position_;
    glm::vec2 localPosition_;

};

#endif // TRANSFORM_H
