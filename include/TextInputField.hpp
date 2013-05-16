#ifndef TEXTINPUTFIELD_H
#define TEXTINPUTFIELD_H

#include <SFML/Graphics.hpp>
#include <string>
#include "GameObject.hpp"

class TextInputField : public GameObject
{
public:
    TextInputField(const std::string & name, const sf::Vector2f & position, const sf::Vector2f & size);
    virtual ~TextInputField();

    void open();
    void close();
    void setLabelText(const std::string & labelText);

	const std::string & getInput()const{return currentText_;}
    bool getOpen()const
    {
        return isOpen_;
    }
    bool isReady()const{return isReady_;}
    void update();
    void setAllowedLetters(const std::string & str);
    void setText(const std::string & txt){currentText_ = txt;}


protected:

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default)const;
private:


    std::string currentText_;
    bool isOpen_;
    bool isReady_;

    sf::Vector2f topLeft;
    sf::Vector2f size;
    sf::RectangleShape box_;
    sf::Text txt_;
    sf::Text label_;


    int labelHeight;

private:
    void detectClick();
    std::string allowedLetters_;


};

#endif // TEXTINPUTFIELD_H
