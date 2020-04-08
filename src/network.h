#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Network.hpp>
#include <vector>

#define PORT 50001

class Network {
private:
	class Client {
	public:
		sf::TcpSocket socket;
		std::string name;
		std::string ip;
	};

	std::vector<Client*> clients;

	sf::TcpSocket socket;
	sf::TcpListener listener;
public:
	Network();

	void host();
	bool connect(std::string ip);

	bool listen();

	std::string getHostIp() const;
	void addClient(std::string ip);
};

#endif
