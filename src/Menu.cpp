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

	start.active = true;

	//LOBBY
	lobby.active = false;

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

	//JOIN
	join.active = false;
	join.origin = sf::Vector2f(700, 0);

	ipConnect.setPosition(0, 200);
	ipConnect.setFont(font);
	ipConnect.setFillColor(sf::Color::White);

	connectBtn.setFont(&font);
	connectBtn.setString("Connect");
	connectBtn.setPosition(sf::Vector2f(700, 300));
	join.addButton(connectBtn);
}

void Menu::run(sf::RenderWindow* window)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
			if (start.active) {
				if (event.type == sf::Event::MouseButtonPressed) {
					if (start.buttons.at(0).isClicked(sf::Mouse::getPosition(*window))) {
						Game* game = new Game(font, "../territories.txt");
						StateManager::getInstance().push(game);
					}
					else if (start.buttons.at(1).isClicked(sf::Mouse::getPosition(*window))) {
						network.host();
						ip.setString("Host IP: " + network.getHostIp());
						std::cout << "Hosting at ip " << ip.getString().getData() << std::endl;
						lobby.active = true;
						start.active = false;
					}
					else if (start.buttons.at(2).isClicked(sf::Mouse::getPosition(*window))) {
						join.active = true;
						start.active = false;
					}
				}
			}
			else if (lobby.active) {

			}
			else if (join.active) {
				if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode != 8) {
						ipConnectStr += event.text.unicode;
						ipConnect.setString(ipConnectStr);
					}
					else {
						ipConnectStr.pop_back();
						ipConnect.setString(ipConnectStr);
					}
				}
				else if (event.type == sf::Event::MouseButtonPressed) {
					if (join.buttons.at(0).isClicked(sf::Mouse::getPosition(*window))) {
						network.connect(ipConnectStr);
					}
				}
			}
	}
	if (lobby.active) {
		if (network.listen()) {
			sf::Text player;
			player.setString("new Player");
			player.setPosition(400, 300);
			player.setFont(font);
			player.setFillColor(sf::Color::White);
			players.push_back(player);
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
		window->draw(ipConnect);
	}
}
