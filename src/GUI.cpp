#include "GUI.hpp"
#include "ConfigLoader.hpp"
#include "FullExpressionAccumulator.hpp"
#include <assert.h>
#include "Time.hpp"
#include "ClientsManager.hpp"
#include "Game.hpp"
#include "LocalPlayerInfo.hpp"

GUI::GUI():
	GameObject("GUI")
{
	//ctor
	instance = this;
	createGUIFromFile();
}

GUI::~GUI()
{
	//dtor
}
void GUI::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(debugField);

	target.draw(fpsField);
	target.draw(connectedPlayerList);
	target.draw(infoField);



}
void GUI::update(){
	LocalPlayerInfo & localInfo = LocalPlayerInfo::instance();
	if(localInfo.character){
		glm::vec2 pos = localInfo.character->getPosition();

		std::stringstream pposStream;
		pposStream << "Pos = " << pos.x <<  ", " << pos.y << ";";
		debugField.setString(pposStream.str());
	}

	std::stringstream stream;
	stream << Time::time << std::endl << 1 / Time::deltaTime;
	fpsField.setString(stream.str());

	std::stringstream stream2;
	assert(game_ != NULL);
	ScoreChange sc = game_->clientsManager_.getScores();

	for(unsigned int i = 0; i < sc.entryCount; i++){

		stream2<< sc.scoreObjects[i].name  << "[" << sc.scoreObjects[i].score << "], ";
	}
	connectedPlayerList.setString(stream2.str());


}
void GUI::createGUIFromFile(){
	ConfigLoader & config = ConfigLoader::instance();

	std::vector<int> res = config.getIntArray("settings,resolution");
	std::string guiTextFields("GUI,textFields,");
	std::string fontPath = config.getString("GUI,fontPath");
	std::vector<int> c = config.getIntArray("GUI,textColor");
	sf::Color textColor = sf::Color(c[0],c[1],c[2]);

	if(!guiFont.loadFromFile(fontPath)){
		FullExpressionAccumulator(std::cout) << "FAILED TO LOAD GUI FONT FROM " << fontPath << "\n";
	}
	else{
		FullExpressionAccumulator(std::cout) << "Loaded font " << fontPath << "\n";
	}
	std::vector<float> pos = config.getFloatArray("GUI,textFields,playerPosition,position");


	assert(c.size() == 3);
	assert(pos.size() == 2);


	debugField.setPosition(pos[0] * res[0],pos[1] * res[1]);
	debugField.setColor(textColor);
	debugField.setString(config.getString("GUI,textFields,playerPosition,defaultString"));
	debugField.setCharacterSize(config.getInt("GUI,textFields,playerPosition,characterSize"));


	pos = config.getFloatArray(guiTextFields + "fps,position");
	assert(pos.size() == 2);
	fpsField.setPosition(pos[0] * res[0], pos[1] * res[1]);

	fpsField.setString(config.getString(guiTextFields + "fps,defaultString"));
	fpsField.setColor(textColor);
	fpsField.setCharacterSize(config.getInt(guiTextFields + "fps,characterSize"));

	pos = config.getFloatArray(guiTextFields + "playerList,position");
	assert(pos.size() == 2);

	connectedPlayerList.setPosition(pos[0] * res[0],pos[1] * res[1]);
	connectedPlayerList.setCharacterSize(config.getInt(guiTextFields+ "playerList,characterSize"));
	connectedPlayerList.setString(config.getString(guiTextFields+"playerList,defaultString"));
	std::vector<float> textFieldsize = config.getFloatArray(guiTextFields + "playerList,size");
	assert(textFieldsize.size() == 2);
	connectedPlayerList.setOrigin(textFieldsize[0], textFieldsize[1]);
	connectedPlayerList.setColor(textColor);

	pos = config.getFloatArray(guiTextFields + "infoField,position");
	infoField.setPosition(pos[0] * res[0],pos[1] * res[1]);
	infoField.setCharacterSize(config.getInt(guiTextFields+ "infoField,characterSize"));
	infoField.setString(config.getString(guiTextFields+"infoField,defaultString"));
	textFieldsize = config.getFloatArray(guiTextFields + "infoField,size");
	assert(textFieldsize.size() == 2);
	infoField.setOrigin(textFieldsize[0]/2, textFieldsize[1]);
	infoField.setColor(textColor);


}

GUI * GUI::instance;
