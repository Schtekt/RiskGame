#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

#define PORT 50001

class Network {
private:
	class Client {
	public:
		sf::TcpSocket socket;
		std::string name;
		bool connected = false;
	};

	std::vector<Client*> clients;

	sf::TcpSocket socket;
	sf::TcpListener listener;

	bool hosting = false;
public:
	Network();

	void host();
	bool connect(std::string ip);
	bool listenConnection();

	sf::Packet listen(int playerIndex);

	void send(sf::Packet packet, int playerIndex);

	void toggleBlocked();

	bool isHost() const;
	std::string getHostIp() const;
	void addClient();
};

#endif
