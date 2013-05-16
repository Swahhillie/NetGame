#ifndef MASTERSERVER_H
#define MASTERSERVER_H

#include <string>
#include <list>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <NetworkEvents.h>



class MasterServer : sf::NonCopyable
{
	public:
		MasterServer();
		virtual ~MasterServer();
		void run();
		void build();
	protected:
	private:

		sf::UdpSocket udpSocket_;
		bool isRunning_;
		bool getRunning()const{return isRunning_;}
		void handleMessage(sf::Packet & packet, const sf::IpAddress & ip, unsigned short port);

		typedef std::list <ServerEntry*> Entries;
		Entries serverEntries_;

		sf::Packet getListOfServers()const;
		sf::Packet addServerEntry(sf::Packet & packet);
};

#endif // MASTERSERVER_H
