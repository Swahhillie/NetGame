#ifndef GUI_HPP
#define GUI_HPP

#include "GameObject.hpp"
#include "TextInputField.hpp"

class Game;

class GUI : public GameObject
{
	public:
		GUI();
		void setGame(Game * game){game_ = game; }
		const sf::Font & getFont()const{return guiFont;}

		static GUI * instance;
		sf::Text infoField; // for login info
	protected:
		virtual void update();
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;

	private:

		virtual ~GUI();

		Game * game_;

		void createGUIFromFile();
		sf::Font guiFont;

		sf::Text debugField;
		sf::Text fpsField;

		void drawConnectedPlayerList()const;
		sf::Text connectedPlayerList;

};

#endif // GUI_HPP
