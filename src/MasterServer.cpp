#include "MasterServer.hpp"
#include "Global_Constants.hpp"
#include <SFML/Network.hpp>
#include "FullExpressionAccumulator.hpp"
#include <assert.h>
#include "DebugSpace.h"

MasterServer::MasterServer()
{
	//ctor
	isRunning_ = true;
}

MasterServer::~MasterServer()
{
	//dtor
	isRunning_ = false;
	udpSocket_.setBlocking(true);

	//clearing for good measure
	for(Entries::iterator it = serverEntries_.begin(); it != serverEntries_.end(); ++it){
		ServerEntry *& e = *it;
		delete e;
		e = NULL;
	}
	serverEntries_.clear();

}
void MasterServer::build(){

	sf::Socket::Status result = udpSocket_.bind(MASTERSERVERPORT);
	assert(result == sf::Socket::Done);
	sf::IpAddress publicAddress = sf::IpAddress::getPublicAddress();
	FullExpressionAccumulator(std::cout) << "Public address of this master server = " << publicAddress.toString() << " : " << MASTERSERVERPORT << "\n";

}
void MasterServer::run(){

	while(getRunning()){

		sf::Packet packet;
		sf::IpAddress remoteAddress;
		unsigned short remotePort;

		FullExpressionAccumulator(std::cout) << " Ready to receive \n";
		sf::Socket::Status status = udpSocket_.receive(packet, remoteAddress, remotePort);
		if(status == sf::Socket::Done){
			handleMessage(packet, remoteAddress, remotePort);
		}
		else{
			FullExpressionAccumulator(std::cout) << "Failed to receive with error " << status << "  \n";
		}
	}

}
void MasterServer::handleMessage(sf::Packet & packet, const sf::IpAddress & ip, unsigned short port){
	bool shouldReply = false;
	sf::Packet replyPacket;

	sf::Int32 command;
	packet >> command;

	if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Master server received command " << Network::commandToString(command) << "\n";
	switch(command){
		case NetworkEvent::SERVER_ENTRY_ADD:

			replyPacket = addServerEntry(packet);
			shouldReply = true;

			break;
		case NetworkEvent::SERVER_ENTRY_GET:

			replyPacket = getListOfServers();
			shouldReply = true;

			break;

	}
	if(shouldReply){
		if(Debug::VERBOSE_MESSAGES)FullExpressionAccumulator(std::cout) << "Replying to " << ip.toString() << " : " << port << "\n";
		Network::sendUdpAndReport(udpSocket_, replyPacket, ip, port);
		//udpSocket_.send(replyPacket, ip, port);
	}
}
sf::Packet MasterServer::addServerEntry(sf::Packet & serverEntryAddEventPacket){
	//can the server entry request be fullfilled?

	//if it can: Create it and reply with accept
	//if it can not: Reply with reject
	sf::Packet replyPacket;
	ServerEntryAdd addEntryEvent;
	serverEntryAddEventPacket >>addEntryEvent;

	//check for duplicate names
	Entries::iterator it = serverEntries_.begin();
	bool okToCreateNewEntry = true;

	for(;it!= serverEntries_.end(); ++it){
		ServerEntry & entry = **it;

		if(entry.name == addEntryEvent.name){
			okToCreateNewEntry = false;
			break;
		}
	}

	if(okToCreateNewEntry){
		//no duplicate found, create the new entry, reply = accept
		ServerEntryAccept serverEntryAcceptEvent;
		replyPacket << serverEntryAcceptEvent;
		ServerEntry * newEntry = new ServerEntry; // cleanup only happends in the destructor for now, 5-4-2013
		newEntry->name = addEntryEvent.name;
		newEntry->ip = addEntryEvent.ip;
		FullExpressionAccumulator(std::cout) << "Server entry add accepted, info: " << newEntry->name << ", ip" << newEntry->ip <<"\n";
		serverEntries_.push_back(newEntry);
	}
	else{

		//a duplicate was found, reply = rejection
		ServerEntryReject serverEntryRejectEvent;
		serverEntryRejectEvent.reason = std::string("Dupe name found");
		FullExpressionAccumulator(std::cout) << "Server entry add rejected, Reason: " << serverEntryRejectEvent.reason << "\n";
		replyPacket << serverEntryRejectEvent;
	}
	//return the reply packet so that the caller can use it
	return replyPacket;
}
sf::Packet MasterServer::getListOfServers()const{
	sf::Packet replyPacket;

	ServerEntryList serverEntryList;
	serverEntryList.entryCount = serverEntries_.size();
	serverEntryList.entries = std::vector<ServerEntry>(serverEntries_.size());

	Entries::const_iterator it= serverEntries_.begin();
	int i = 0;
	for(; it != serverEntries_.end(); ++it, i++){
		ServerEntry & entry = **it;
		serverEntryList.entries[i] = entry; // copy ctr into entry slot

	}
	replyPacket << serverEntryList;
	std::cout << serverEntryList.getFormattedSting();
	return replyPacket;
}
