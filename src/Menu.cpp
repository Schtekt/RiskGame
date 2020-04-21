#include "Menu.h"

Menu::Menu()
{
	font.loadFromFile("../arial.ttf");

	//START
	start.origin = sf::Vector2f(700, 200);

	gameBtn.setFont(&font);
	gameBtn.setString("Game");
	gameBtn.setPosition(sf::Vector2f(0, 0));
	start.addButton(gameBtn);

	hostBtn.setFont(&font);
	hostBtn.setString("Host");
	hostBtn.setPosition(sf::Vector2f(0, 100));
	start.addButton(hostBtn);

	joinBtn.setFont(&font);
	joinBtn.setString("Join");
	joinBtn.setPosition(sf::Vector2f(0, 200));
	start.addButton(joinBtn);

	name.setPosition(sf::Vector2f(200, 200));
	name.setFont(&font);
	name.setString("Unamed Player");
	start.addButton(name);

	start.active = true;

	//LOBBY
	lobby.active = false;
	lobby.origin = sf::Vector2f(200, 200);

	ip.setString("No ip found!");
	ip.setPosition(800, 100);
	ip.setFont(font);
	ip.setFillColor(sf::Color::White);

	sf::Text host;
	host.setString("Host");
	host.setPosition(400, 200);
	host.setFont(font);
	host.setFillColor(sf::Color::White);
	players.push_back(host);

	addBtn.setFont(&font);
	addBtn.setString("+");
	addBtn.setPosition(sf::Vector2f(150, 0));
	lobby.addButton(addBtn);

	//JOIN
	join.active = false;
	join.origin = sf::Vector2f(700, 0);

	connectBtn.setFont(&font);
	connectBtn.setString("Connect");
	connectBtn.setPosition(sf::Vector2f(0, 400));
	join.addButton(connectBtn);

	ipConnect.setPosition(sf::Vector2f(0, 200));
	ipConnect.setFont(&font);
	ipConnect.setString("255.255.255.255");
	join.addButton(ipConnect);
}

void Menu::run(sf::RenderWindow* window)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
			if (start.active) {
				if (event.type == sf::Event::MouseButtonPressed) {
					//Game
					if (start.buttons.at(0).isClicked(sf::Mouse::getPosition(*window))) {
						Game* game = new Game(font, "../territories.txt");
						StateManager::getInstance().push(game);
					}
					//Host
					else if (start.buttons.at(1).isClicked(sf::Mouse::getPosition(*window))) {
						network.host();
						ip.setString("Host IP: " + network.getHostIp());
						std::cout << "Hosting at ip " << ip.getString().getData() << std::endl;
						players.at(0).setString(start.buttons.at(3).getString());
						lobby.active = true;
						start.active = false;
					}
					//Join
					else if (start.buttons.at(2).isClicked(sf::Mouse::getPosition(*window))) {
						join.active = true;
						start.active = false;
					}
				}
				if (event.type == sf::Event::TextEntered) {
					inputStr = start.buttons.at(3).getString();


					if (event.text.unicode != 8) {
						inputStr += event.text.unicode;
						start.buttons.at(3).setString(inputStr);
					}
					else if (inputStr.size() > 0) {
						inputStr.pop_back();
						start.buttons.at(3).setString(inputStr);
					}
				}
			}
			else if (lobby.active) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (lobby.buttons.at(0).isClicked(sf::Mouse::getPosition(*window))) {
						network.addClient();
						sf::Text player;
						player.setString("waiting player!");
						player.setPosition(400, 200 + players.size() * 100);
						player.setFont(font);
						player.setFillColor(sf::Color::White);
						players.push_back(player);
					}
				}
			}
			else if (join.active) {
				if (event.type == sf::Event::TextEntered) {
					inputStr = join.buttons.at(1).getString();


					if (event.text.unicode != 8) {
						inputStr += event.text.unicode;
						join.buttons.at(1).setString(inputStr);
					}
					else if(inputStr.size() > 0){
						inputStr.pop_back();
						join.buttons.at(1).setString(inputStr);
					}
				}
				else if (event.type == sf::Event::MouseButtonPressed) {
					if (join.buttons.at(0).isClicked(sf::Mouse::getPosition(*window))) {
						if (network.connect(join.buttons.at(1).getString())) {
							lobby.active = true;
							join.active = false;

							sf::Packet packet;
							packet << PLAYER_NAME_HOST;
							packet << start.buttons.at(3).getString();
							network.send(packet, 0);
						}
					}
				}
			}
	}
	if (lobby.active) {
		if (network.isHost()) {
			if (network.listenConnection()) {
			}
			for (int i = 1; i < players.size(); i++) {
				sf::Packet packet = network.listen(i - 1);
				if (packet.getDataSize() > 0) {
					int id;
					packet >> id;
					switch (id) {
					case PLAYER_NAME_HOST: {
						std::string name;
						packet >> name;
						players.at(i).setString(name);
						for (int i = 0; i < players.size(); i++) {
							sf::Packet packet;
							std::string name;
							name = players.at(i).getString();
							packet << PLAYER_NAME << i << name;
							network.send(packet, players.size() - 2);
						}
						break;
					}
					}
				}
			}
		} else {
			sf::Packet packet = network.listen(0);
			if (packet.getDataSize() > 0) {
				int id;
				packet >> id;
				switch (id) {
				case PLAYER_NAME: {
					int playerIndex;
					std::string name;
					packet >> playerIndex;
					packet >> name;
					if (playerIndex == players.size()) {
						sf::Text newPlayer;
						newPlayer.setString("New Player");
						newPlayer.setPosition(400, 200 + players.size() * 100);
						newPlayer.setFont(font);
						newPlayer.setFillColor(sf::Color::White);
						players.push_back(newPlayer);
					}
					players.at(playerIndex).setString(name);
					break;
					}
				}
			}
		}
	}
}

void Menu::render(sf::RenderWindow* window)
{
	if (start.active) {
		start.render(window);
	}
	else if (lobby.active) {
		lobby.render(window);
		window->draw(ip);
		for (int i = 0; i < players.size(); i++)
			window->draw(players.at(i));
	}
	else if (join.active) {
		join.render(window);
	}
}
