#include "Scene.hpp"
#include "Time.hpp"
#include "FullExpressionAccumulator.hpp"
#include <sstream>
#include <math.h>
#include "ControlledCharacter.hpp"
#include "Transform.hpp"
#include "GUI.hpp"
#include "NetworkedCharacter.hpp"
#include <glm/glm.hpp>
#include "Level.hpp"
#include "Game.hpp"
#include "TargetBlock.hpp"
#include <assert.h>
#include "ConfigLoader.hpp"

Scene::Scene(sf::RenderWindow & window, Game & game):
    window_(window),
    game_(game),
    root_(new GameObject("root")),
    character(NULL)
{
    //ctor
    //leaking character pointer, it doesn't get destroyed, 1 - 4 - 2013, temporary
    view_.reset(sf::FloatRect(0, 0, window_.getSize().x, window_.getSize().y));

    gui_ = new GUI();
    targetBlockTexture = new sf::Texture();
    bool result = targetBlockTexture->loadFromFile(ConfigLoader::instance().getString("textures,targetBlockTexture"));
    assert(result);
    //gui->transform.setParent(&root_->transform);

}

Scene::~Scene()
{
    //dtor
    GameObject::destroy(*root_);
    GameObject::destroy(*gui_);
    root_->cleanup();//final cleanup
    gui_->cleanup();
    delete targetBlockTexture;


}
void Scene::loadLevel(const std::string & name)
{
    lvl_.loadLevelNamed(name);
    tempSprite.setTexture(*lvl_.getTexture(), true);
    targets_.clear();
}

void Scene::createCharacter()
{
    character = new ControlledCharacter("PlayerCharacter", sf::Color::Blue);
    character->transform.setParent(&root_->transform);
}
//------------------------public functions
NetworkedCharacter * Scene::createNetworkedCharacter(std::string name, const sf::Color & color)
{

    NetworkedCharacter * netPlayer = new NetworkedCharacter(name, color);
    netPlayer->transform.setParent(&root_->transform);
    networkedCharacters_.push_back(netPlayer);
    return netPlayer;
}
void Scene::createTarget(unsigned int targetId, const glm::vec2 & position)
{
    std::string tIdString = "";
    std::stringstream stream;
    stream << targetId;
    stream >> tIdString;

    TargetBlock * tb = new TargetBlock("tb" + tIdString, *targetBlockTexture);
    tb->transform.setLocalPosition(position);
    std::cout << "Created a target at position " <<  position.x << ", " << position.y << std::endl;
    addToScene(*tb);

    Targets::iterator it = targets_.find(targetId);
    assert(it == targets_.end());

    targets_[targetId] = tb;
}
void Scene::removeTarget(unsigned int targetId)
{
    Targets::iterator it = targets_.find(targetId);
    assert(it != targets_.end()); //no target with this id
    GameObject::destroy(*it->second);
}
void Scene::removeNetworkedCharacter(std::string name)
{
    for(std::vector<NetworkedCharacter*>::iterator it = networkedCharacters_.begin(); it!= networkedCharacters_.end() ; ++it)
    {
        NetworkedCharacter *& netPlayer = *it;
        if(netPlayer->name == name)
        {

            GameObject::destroy(*netPlayer);
            netPlayer = NULL;
            networkedCharacters_.erase(it);
            return;

        }

    }
}
void Scene::update()
{
    root_->ghostUpdate();
    gui_->ghostUpdate();
    cleanup();
}
void Scene::cleanup()
{
    root_->cleanup();
}
void Scene::draw()
{
    window_.clear(sf::Color::White);

    //focus camera on character
    if(game_.getGameState() == Game::Playing)
    {
        if(character)
        {

            glm::vec2 charPos = character->transform.getLocalPosition();
            view_.setCenter(charPos.x, charPos.y);
            window_.setView(view_);

        }
        window_.draw(tempSprite);
    }

    root_->ghostDraw(window_, sf::RenderStates::Default);

    window_.setView(window_.getDefaultView());
    gui_->ghostDraw(window_, sf::RenderStates::Default);

    //window_.setView(window_.getDefaultView());
    window_.display();
}
void Scene::addToScene(GameObject & go)
{
    go.transform.setParent(&root_->transform);
}
