#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <SFML/Graphics.hpp>
#include <map>
#include <GLM/GLM.hpp>
class Level
{
public:
    Level();
    virtual ~Level();
    typedef std::vector<int> Ints;


    const std::vector<int> & getLevelArray()const
    {
        return levelArray_;
    }

    bool getLoaded()const
    {
        return isLoaded_;
    }

    int toIndex(glm::vec2 position)const;
    int getValue(int index)const{return levelArray_[index];}
    bool isWalkable(int value)const { return value != 30;} //!----------------------------------------------------TEMPORARY
    void loadLevelNamed(const std::string & name);
    glm::vec2 getStartPos()const;
    const std::vector<glm::vec2> & getObjectPositions()const{return objectPositions_;}
    std::string getLoadedLevelName()const{return loadedLevelName_;}
    std::string levelToString();
    const sf::Texture * getTexture(); //creates and returns a level texture

private:
    Ints levelArray_;
    sf::Texture * texture_;
    unsigned int width_;
    unsigned int height_;
    unsigned int tileSize_;
    std::vector<glm::vec2> objectPositions_;
    bool isLoaded_;
    int startX_;
    int startY_;

    void createTexture();
    void parseObjectPositions(const std::string & scope);
    sf::Color getColor(int i, int j);
    void parseColors();

    void writeTile(sf::Image * img, int x, int y, int tileSize, const sf::Color & color);
    typedef std::map<int, sf::Color> ColorMap;
    ColorMap colorMap_;
    std::string loadedLevelName_;




};

#endif // LEVEL_H
