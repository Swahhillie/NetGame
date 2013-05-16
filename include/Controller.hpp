#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <glm/glm.hpp>

class Controller
{
public:
    Controller(sf::RenderWindow & window);
    virtual ~Controller();

    void update();
    bool running()const
    {
        return isRunning_;
    }

    virtual const std::string getName()
    {
        return "Controller";
    }
    void stopRunning()
    {
        isRunning_ = false;
    }

    //keyboard
    static bool getKey(sf::Keyboard::Key keyCode);
    static bool getKeyDown(sf::Keyboard::Key keyCode);
    static bool getKeyUp(sf::Keyboard::Key keyCode);

    //mouse
    static bool getButton(sf::Mouse::Button button);
    static bool getButtonDown(sf::Mouse::Button button);
    static bool getButtonUp(sf::Mouse::Button button);

/** \brief If a text event has occurred in the last frame txt will be set to the captured text.
 *
 * \param string will be set to caught characters of the last frame.
 * \return If any text was entered will return true.
 */

	static bool getTextEntered(std::string & txt);

    static glm::vec2 getDeltaMouse();

    static glm::vec2 mousePosition;
    static glm::vec2 lastMousePosition;

protected:
private:

    sf::RenderWindow & window_;
    bool isRunning_;


private:
    void refresh();
    void handleEvent(const sf::Event & event);

    static Controller * instance;
    std::vector<bool> pressedKeys;
    std::vector<bool> lastKeys;

    std::vector<bool> pressedButtons;
    std::vector<bool> lastButtons;

	std::string capturedText;

    bool mousePressed;
};
#endif // CONTROLLER_H
