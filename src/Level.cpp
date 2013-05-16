#include "Level.hpp"
#include "ConfigLoader.hpp"

#include <sstream>
#include <assert.h>
#include <GLM/GLM.hpp>
#include <math.h>

Level::Level():texture_(NULL),
    width_(0),
    height_(0),
    tileSize_(0),
    isLoaded_(false),
    startX_(0),
    startY_(0)
{
    //ctor
}

Level::~Level()
{
    //dtor
}
int Level::toIndex(glm::vec2 position)const
{
    int x = floorf(position.x / tileSize_);
    int y = floorf(position.y / tileSize_);
    if(x < 0 || x == width_ || y < 0 || y == height_) return -1; //keep in bounds
    int ind = y * width_ + x;
    //std::cout <<"pos x = " << position.x << ", pos y = " << position.y <<  ", x = " << x << ", y = " << y << ", ind = " << ind << ", return = " << (ind >= 0 && ind < width_ * height_? ind : -1) << std::endl;
    return ind >= 0 && ind < width_ * height_? ind : -1;

}
glm::vec2 Level::getStartPos()const
{
	return glm::vec2(startX_ * tileSize_, startY_ * tileSize_);
}
void Level::loadLevelNamed(const std::string & str)
{
    std::cout << "START LOADING " << str << std::endl;
    if(texture_)
    {
        delete texture_;
        texture_ = NULL;
    }
    loadedLevelName_ = str;
    std::string levelScope = "levels," + str + ",";
    width_ = ConfigLoader::instance().getInt(levelScope + "width");
    height_ = ConfigLoader::instance().getInt(levelScope + "height");
    tileSize_ = ConfigLoader::instance().getInt(levelScope + "tileSize");
    levelArray_ = ConfigLoader::instance().getIntArray(levelScope + "data", false);

    std::vector<int> startPos = ConfigLoader::instance().getIntArray(levelScope + "start");

    parseObjectPositions(levelScope);
    assert(startPos.size() == 2);
    startX_ = startPos[0];
    startY_ = startPos[1];

    parseColors();
    assert(levelArray_.size() == width_ * height_);
    isLoaded_ = true;
    std::cout << "LOADING SUCCESFULL" << std::endl;


    //std::cout << levelToString() << std::endl;
}
const sf::Texture * Level::getTexture()
{
    if(texture_ == NULL)
    {
        createTexture();
        assert(texture_ != NULL);
        return texture_;
    }
    else
    {

        return texture_;
    }
}
void Level::parseColors()
{
    colorMap_.clear();
    std::vector<int> availableColors = ConfigLoader::instance().getIntArray("colors,available");
    std::cout << "Colors to parse = " << availableColors.size() << std::endl;

    for(unsigned int i = 0; i < availableColors.size(); i++)
    {
        std::stringstream stream;
        std::string intString = "";
        stream << availableColors[i];
        stream >> intString;

        std::vector<int> values = ConfigLoader::instance().getIntArray("colors," + intString);
        assert(values.size() == 4);
        sf::Color c(values[0], values[1], values[2], values[3]);
        colorMap_[availableColors[i]] = c;
    }
}
void Level::parseObjectPositions(const std::string & scope){
	objectPositions_.clear();
	Ints ints = ConfigLoader::instance().getIntArray(scope + "objectPositions");
	assert(ints.size() == width_ * height_);
	unsigned int objectCount = 0;
	for(unsigned int i = 0 ; i < ints.size() ;i++)
	{
		if(ints[i] != 0)
			objectPositions_.push_back(glm::vec2((i % width_) * tileSize_, (i / width_) * tileSize_));
	}
	std::cout << "There are " << objectCount << " objects to get in level " << scope << std::endl;

}
std::string Level::levelToString()
{
    std::stringstream stream;
    stream << "[";
    for(unsigned int i = 0; i < width_; i ++)
    {

        for(unsigned int j = 0 ; j < height_ ; j++)
        {

            stream << levelArray_[i * width_ + j] << ",";

        }

        stream << "\n";
    }
    stream << "]";
    return stream.str();
}
void Level::createTexture()
{
    assert(texture_ == NULL);
    sf::Image * image = new sf::Image;
    image->create(width_ * tileSize_, height_ * tileSize_, sf::Color::Red);
    for(unsigned int i = 0 ; i < width_; i++)
    {

        for(unsigned int j = 0; j < height_; j++)
        {

            writeTile(image, i * tileSize_,j * tileSize_,tileSize_,getColor(j,i));

        }

    }
    texture_ = new sf::Texture;
    if(!texture_->loadFromImage(*image))
    {
        std::cout << " Creating the texture failed " << std::endl;
    }
    delete image;

}
void Level::writeTile(sf::Image * img, int x, int y, int tileSize_, const sf::Color & color)
{
    for(int i = 0 ; i < tileSize_; i++)
    {
        for(int j = 0; j < tileSize_; j++)
        {
            img->setPixel(x + i, y + j, color);
        }
    }
}
sf::Color Level::getColor(int i, int j)
{
    return colorMap_[levelArray_[i * width_ + j]];
}
