#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <glm/glm.hpp>
class ControlledCharacter;
class NetworkedCharacter;
class GUI;
class Game;
class TargetBlock;
#include "Level.hpp"

class Scene
{
public:
    Scene(sf::RenderWindow & window, Game & game);
    virtual ~Scene();

    void update();
    void cleanup();
    void draw();

    void createCharacter();
    NetworkedCharacter * createNetworkedCharacter(std::string name, const sf::Color & color);
    void removeNetworkedCharacter(std::string name);

    void addToScene(GameObject & go);

	void loadLevel(const std::string & name);

	const Level * getLevel()const{return &lvl_;}

	void createTarget(unsigned int targetId, const glm::vec2 & pos);
	void removeTarget(unsigned int targetId);


protected:
private:
	sf::Sprite tempSprite;
	GUI * gui_;
    sf::RenderWindow & window_;
    Game & game_;

    sf::Texture * targetBlockTexture;
    typedef std::map<unsigned int, TargetBlock*> Targets;
    Targets targets_;
	Level lvl_;
private:
    sf::View view_;
    GameObject * root_; // scene root

	ControlledCharacter * character;
	std::vector<NetworkedCharacter*> networkedCharacters_;


};

#endif // SCENE_H
