#include "network.h"

Network::Network()
{
}

void Network::host()
{
	hosting = true;

	if (listener.listen(PORT) != sf::Socket::Done) {

	}
	listener.setBlocking(false);
}

bool Network::connect(std::string ip)
{
	sf::Socket::Status status = socket.connect(ip, PORT);
	if (status != sf::Socket::Done)
	{
		return false; //Connection failed
	}

	return true;
}

bool Network::listenConnection()
{
	for (int i = 0; i < clients.size(); i++) {
		if (!clients.at(i)->connected) {
			if (listener.accept(clients.at(i)->socket) == sf::Socket::Done) {
				return true;
			}
		}
	}
	
	return false;
}

sf::Packet Network::listen(int playerIndex)
{
	sf::Packet packet;
	if (hosting) {
		if (playerIndex >= 0 && playerIndex < clients.size()) {
			clients.at(playerIndex)->socket.receive(packet);
			if(packet.getDataSize() > 0)
				std::cout << "Received packet of size " << packet.getDataSize() << " bytes" << std::endl;
		}
	}
	else {
		socket.receive(packet);
		if (packet.getDataSize() > 0)
			std::cout << "Received packet of size " << packet.getDataSize() << " bytes" << std::endl;
	}
	return packet;
}

void Network::send(sf::Packet packet, int playerIndex)
{
	if (hosting)
		clients.at(playerIndex)->socket.send(packet);
	else
		socket.send(packet);
	std::cout << "Sending packet with size " << packet.getDataSize() << " bytes" << std::endl;
}

void Network::toggleBlocked()
{
	listener.setBlocking(!listener.isBlocking());
}

bool Network::isHost() const
{
	return hosting;
}

std::string Network::getHostIp() const
{
	return socket.getRemoteAddress().getPublicAddress().toString();
}

void Network::addClient()
{
	clients.push_back(new Client);
	clients.at(clients.size() - 1)->socket.setBlocking(false);
}
