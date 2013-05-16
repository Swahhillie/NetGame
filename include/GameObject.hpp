#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

class Transform;

class GameObject : sf::NonCopyable, sf::Drawable
{
    friend class Scene;
    friend class Transform;

public:
    explicit GameObject(const std::string & name);

    virtual ~GameObject();

    static void destroy(GameObject & go);

    std::string name;
    Transform & transform;


protected:
	bool isMarkedForDestroy;
    virtual void start() {} //start is called before the first update
    virtual void update() {} //update is called every frame
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const; //draw is where the object draws itself
    virtual void onDestroy();


private:
    void ghostUpdate();
    void ghostDraw(sf::RenderTarget& target, sf::RenderStates states)const;
    void cleanup();
    bool started_;

};

#endif // GAMEOBJECT_H
