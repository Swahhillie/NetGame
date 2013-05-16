#ifndef CLIENTSMANAGER_H
#define CLIENTSMANAGER_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Client.hpp"
#include "NetworkEvents.h"
class NetworkedCharacter;
class Level;


class ClientsManager
{
	public:
		explicit ClientsManager(bool isServerSide = false);
		virtual ~ClientsManager();

		void removeClient(const std::string & name);
		bool addClient(const std::string & name, const sf::Color & color = sf::Color::Blue, const sf::IpAddress & ip = sf::IpAddress::None, unsigned short port = 0);
		void bindCharacterToClient(std::string name, NetworkedCharacter * character);
		Client * getClient(const std::string & name);
		std::string getRejectionReason()const{return rejectionReason_;}
        /** \brief Extrapolate clients
         *
         * \return
         *	Extrapolate client position based on :
         *  current key state
         *  current velocity
         *  current position
         *  current acceleration
         *  time since last update
         */

		void extrapolatePositions();
		void updateClient(const PlayerStateUpdate & puts);

		typedef std::map<std::string,Client*> Clients;
		Clients & getClients();
		//std::vector<sf::IpAddress> getAddresses()const;
		sf::Packet getClientUpdates()const;
		void setActiveLevel(const Level * level);
		const Level & getActiveLevel()const{return *activeLevel_;}
		void resetScores();
		ScoreChange getScores();
		void setScore(const std::string & name, int score);
	protected:

	private:
		//player statistics
		float playerAcceleration;
		float playerMoveSpeed;
		//client score setter



	private:
		ClientsManager(const ClientsManager& other);
		ClientsManager& operator=(const ClientsManager& other);

	private:
	  //!handeling of clients
		void makeStep(Client & client, float secondsToCompute);
		void extrapolateClient(Client & client);

		Clients clients_;
		bool collision(glm::vec2 position);
		void setToStartPosition(Character * character);
		bool isServerSide_;
		const Level * activeLevel_;

		std::string rejectionReason_; // reason for the last rejection
};

#endif // CLIENTSMANAGER_H
