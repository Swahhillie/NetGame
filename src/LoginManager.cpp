#include "LoginManager.hpp"
#include "Scene.hpp"
#include <assert.h>
#include "TextInputField.hpp"
#include <iostream>
#include "GUI.hpp"
#include "NetworkEvents.h"
#include "ConfigLoader.hpp"
#include "Button.hpp"

LoginManager::LoginManager(Scene & scene):
    state_(Idle),
    scene_(scene),
    activeInput(NULL),
    loginButton(NULL),
    registerButton(NULL)
{
    //ctor
    isReady_ = false;
}

LoginManager::~LoginManager()
{
    //dtor
}
void LoginManager::setInfo(const std::string & str)
{
    GUI::instance->infoField.setString(str);
}

void LoginManager::startLogin()
{
    assert(state_ == Idle);
    setState(Name);
    isReady_ = false;
}

void LoginManager::update()
{


    if(state_ == Name)
    {
        assert(activeInput != NULL);
        if(activeInput->isReady())
        {
            std::cout << "input was " << activeInput->getInput() << std::endl;
            loginData_.name = activeInput->getInput();
            setState(Password);
        }
    }
    else if(state_ == Password)
    {
        assert(activeInput != NULL);
        if(activeInput->isReady())
        {
            std::cout << "input was " << activeInput->getInput() << std::endl;
            loginData_.password = activeInput->getInput();
            setState(ServerAddress);
        }

    }
    else if (state_ == ServerAddress)
    {
        assert(activeInput != NULL);
        if(activeInput->isReady())
        {
            std::cout << "input was " << activeInput->getInput() << std::endl;
            loginData_.ipAddress = activeInput->getInput();

            setState(RegisterUser);

        }

    }
    else if (state_ == RegisterUser)
    {
        assert(loginButton != NULL);
        assert(registerButton != NULL);
        if(loginButton->wasClicked())
        {

            loginData_.isRegistration = false;
            setState(Idle);
            isReady_ = true;
        }
        else  if(registerButton->wasClicked())
        {
            loginData_.isRegistration = true;
            setState(Idle);
            isReady_ = true;
        }

    }

}
void LoginManager::setState(LoginState state)
{

    if(activeInput)
    {
        GameObject::destroy(*activeInput);
        activeInput = NULL;
    }
    if(loginButton)
    {
        GameObject::destroy(*loginButton);
        loginButton = NULL;
    }
    if(registerButton)
    {
        GameObject::destroy(*registerButton);
        registerButton = NULL;
    }


    assert(activeInput == NULL && registerButton == NULL && loginButton == NULL);


    switch(state)
    {
    case Name:
        activeInput = new TextInputField("nameField", sf::Vector2f(200,50), sf::Vector2f(200,50));
        scene_.addToScene(*activeInput);
        activeInput->setLabelText("Enter your username");
        activeInput->open();
        if(Debug::AUTOFILL_LOGGIN)
            activeInput->setText(ConfigLoader::instance().getString("defaults,username"));
        break;
    case Password:

        activeInput = new TextInputField("passwordField", sf::Vector2f(200,50), sf::Vector2f(200,50));
        scene_.addToScene(*activeInput);
        activeInput->setLabelText("Enter your password");
        activeInput->open();
        if(Debug::AUTOFILL_LOGGIN)
            activeInput->setText(ConfigLoader::instance().getString("defaults,password"));

        break;
    case ServerAddress:

        activeInput = new TextInputField("passwordField", sf::Vector2f(200,50), sf::Vector2f(200,50));
        scene_.addToScene(*activeInput);
        activeInput->setLabelText("Enter server ip address");
        activeInput->open();
        if(Debug::AUTOFILL_LOGGIN)
            activeInput->setText(ConfigLoader::instance().getString("defaults,serverAddress"));

        break;
    case RegisterUser:
        loginButton = new Button("loginButton", sf::Vector2f(200,50), sf::Vector2f(200,50));
        scene_.addToScene(*loginButton);
        loginButton->setText("Login");

        registerButton = new Button("registerButton", sf::Vector2f(200,100), sf::Vector2f(200,50));
        scene_.addToScene(*registerButton);
        registerButton->setText("Register");
        GUI::instance->infoField.setString("Select login or register");

        break;
    case Idle:
        GUI::instance->infoField.setString("");
        break;
    }

    state_ = state;
}
