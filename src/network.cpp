#include "network.h"

Network::Network()
{
	if (listener.listen(PORT) != sf::Socket::Done) {

	}
	listener.setBlocking(false);
}

void Network::host()
{
	addClient(sf::IpAddress::getLocalAddress().toString());
	clients.push_back(new Client);
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

bool Network::listen()
{
	if (listener.accept(clients.at(clients.size() - 1)->socket) == sf::Socket::Done) {
		clients.push_back(new Client);
		return true;
	}

	return false;
}

std::string Network::getHostIp() const
{
	return clients.at(0)->socket.getRemoteAddress().getLocalAddress().toString();
}

void Network::addClient(std::string ip)
{
	clients.push_back(new Client);
}
