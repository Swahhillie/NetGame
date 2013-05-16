#include "ClientsManager.hpp"
#include "Client.hpp"
#include "FullExpressionAccumulator.hpp"
#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>
#include "ConfigLoader.hpp"
#include "Time.hpp"
#include "NetworkedCharacter.hpp"
#include "Level.hpp"
#include "LocalPlayerInfo.hpp"

ClientsManager::ClientsManager(bool serverSide):isServerSide_(serverSide),activeLevel_(NULL)
{
    //ctor
    playerAcceleration = ConfigLoader::instance().getFloat("settings,playerCharacter,acceleration");

    playerMoveSpeed = ConfigLoader::instance().getFloat("settings,playerCharacter,moveSpeed");
    //playerMoveSpeed = ConfigLoader::instance().getFloatSetting("playerCharacter","moveSpeed");
    FullExpressionAccumulator(std::cout) << "Server side player acceleration = " << playerAcceleration << ", movespeed = " << playerMoveSpeed << "\n";
}

ClientsManager::~ClientsManager()
{
    //dtor
    //iterate of the clients and destroy
    for(Clients::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        Client *& c = it->second;
        delete c;
        c = NULL;
    }
    clients_.clear(); //clearing the map for good measure

}

ClientsManager::ClientsManager(const ClientsManager& other)
{
    //copy ctor
}

ClientsManager& ClientsManager::operator=(const ClientsManager& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
bool ClientsManager::addClient(const std::string & name, const sf::Color & color, const sf::IpAddress & ip, unsigned short port)
{
    if(clients_.find(name) == clients_.end())
    {
        Client * c = new Client(name, color, ip);
        c->port = port;
        clients_[name] = c;
        if(isServerSide_)
        {
            c->character = new Character(); //this is serverside, client is data only, client side it will be set by onnection events
            setToStartPosition(c->character);
        }

        FullExpressionAccumulator(std::cout) << "Added '" << name << "' to clients \n";
        return true;
    }
    else
    {
        FullExpressionAccumulator(std::cout) <<"' " << name << " ' could not be added to clients, there is already a player named " << name <<  "\n";
        rejectionReason_ = "Duped username";
        return false;
    }

}
void ClientsManager::bindCharacterToClient(std::string name, NetworkedCharacter * character)
{
    Client * client = getClient(name);
    assert(client != NULL);
    assert(client->character == NULL);
    client->character = character;
    setToStartPosition(character);
}
void ClientsManager::removeClient(const std::string & name)
{
    Clients::iterator it = clients_.find(name);
    if(it != clients_.end())
    {
        Client *& c = it->second;
        delete c;
        c = NULL;
        clients_.erase(it);
        FullExpressionAccumulator(std::cout) << "Removed '" << name <<" 'from clients \n";
    }
    else
    {
        FullExpressionAccumulator(std::cout) << "No client named '" << name <<" ' to remove from clients \n";
    }

}
Client * ClientsManager::getClient(const std::string & name)
{
    Clients::iterator it = clients_.find(name);
    if(it != clients_.end())
    {
        return (it->second);
    }
    else
    {
        FullExpressionAccumulator(std::cout) << "No client named '" << name << "'\n";
        return NULL;
    }
}
void ClientsManager::extrapolatePositions()
{
    //extrapolate the position of each client based on its keystate and last known velocity and time
    for(Clients::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        Client & client = *(it->second);
        extrapolateClient(client);
        //float playerMoveSpeed;

    }
}
void ClientsManager::extrapolateClient(Client & client)
{


    while(!client.toProcessUpdates.empty())
    {
        //retrace the events that occured up to the last update from the player
        PlayerStateUpdate & cur = client.toProcessUpdates.front();

        makeStep(client, cur.receiveTime - client.time ); // simulate client up to the currently processing event
        //std::cout << " event send time:" << cur.sendTime << std::endl;

        //update the state of the client based on the last received event, making it ready for the next step
        client.keyState = cur.keyState;
        if(!isServerSide_)client.character->setPosition(cur.position);
        client.toProcessUpdates.pop(); // destroys cur

    }
    assert(client.toProcessUpdates.empty());
    //extrapolate the most recent position based on last known state
    if(!isServerSide_){
		if(LocalPlayerInfo::instance().name == client.name)
			return;
    }
    makeStep(client, Time::deltaTime);
    client.time = Time::time; // this is your last update time now, ignoring lag
    //client.time = Time::time; // client is simulated up to this point



}
void ClientsManager::makeStep(Client & client, float secondsToCompute)
{
    glm::vec2 lastPos = client.character->getPosition();
    client.character->makeStep(client.keyState, client.moveSpeed, secondsToCompute);
    glm::vec2 newPos = client.character->getPosition();
    if(collision(newPos))client.character->setPosition(lastPos);
}
void ClientsManager::updateClient(const PlayerStateUpdate & puts)
{
    Client * client = getClient(puts.name);
    if(client)
    {
        client->toProcessUpdates.push(puts);
    }
}

ClientsManager::Clients & ClientsManager::getClients()
{
    return clients_;
}

sf::Packet ClientsManager::getClientUpdates()const
{
    sf::Packet pack;
    PlayerUpdateToClient playerUpdates;
    playerUpdates.serverTime = Time::time;
    playerUpdates.entryCount = clients_.size();
    playerUpdates.players = std::vector<PlayerStateUpdate>(clients_.size());

    int i = 0;
    for(Clients::const_iterator it = clients_.begin(); it != clients_.end(); ++it, ++i)
    {
        const Client & cl = *(it->second);
        PlayerStateUpdate state;
        state.receiveTime = Time::time;
        state.keyState = cl.keyState;
        state.name = cl.name;
        state.position = cl.character->getPosition();
        playerUpdates.players[i] = state;
    }

    pack << playerUpdates;
    // std::cout << "packet byte size = " << sizeof(pack) << std::endl;
    return pack;
}
bool ClientsManager::collision(glm::vec2 position)
{

    assert(activeLevel_ != NULL);

    int index = activeLevel_->toIndex(position);
    if(index == -1) return true; // out of the map
    else
    {

        int value = activeLevel_->getValue(index);
        return activeLevel_->isWalkable(value);
    }
}
void ClientsManager::setToStartPosition(Character * character)
{
    assert(activeLevel_ != NULL);
    assert(character != NULL);
    character->setPosition(activeLevel_->getStartPos());
}
void ClientsManager::setActiveLevel(const Level * level)
{
    assert(activeLevel_ == NULL || activeLevel_ == level);
    assert(level != NULL);
    activeLevel_ = level;
    for(Clients::iterator it = clients_.begin(); it != clients_.end(); ++it){

		setToStartPosition(it->second->character );
    }
}
void ClientsManager::resetScores()
{
	Clients::iterator it = clients_.begin();
    for(; it!=clients_.end(); ++it)
    {
        Client & c = *it->second;
		c.score = 0;
    }
}
ScoreChange ClientsManager::getScores()
{
    ScoreChange changeScore;
    changeScore.entryCount = clients_.size();
    changeScore.scoreObjects = std::vector<ScoreObject>(clients_.size());
    Clients::iterator it = clients_.begin();
    int i = 0;
    for(; it!=clients_.end(); ++it, ++i)
    {
        Client & c = *it->second;
		changeScore.scoreObjects[i].name = c.name;
		changeScore.scoreObjects[i].score = c.score;

    }
    return changeScore;



}
void ClientsManager::setScore(const std::string & name, int score){
	assert(isServerSide_ == false);
	Client * c = getClient(name);
	assert(c != NULL);
	c->score = score;
}
