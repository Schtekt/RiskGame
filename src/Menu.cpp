#include "Menu.h"

Menu::Menu()
{
	font.loadFromFile("../arial.ttf");

	button.setFont(&font);
	button.setString("Game");
	button.setPosition(sf::Vector2f(400, 100));
}

void Menu::run(sf::RenderWindow* window)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if (event.type == sf::Event::MouseButtonPressed) {
			if (button.isClicked(sf::Mouse::getPosition(*window))) {
				Game* game = new Game(font);
				game->LoadTerritories("../territories.txt");
				//=============TESTING=============================
				game->AddPlayer("Jacob Andersson", sf::Color::Red);
				game->AddPlayer("Leo Wikström", sf::Color::Green);
				game->AddPlayer("Tim Johansson", sf::Color::Blue);
				game->AddPlayer("Anton Åsbrink", sf::Color::Yellow);
				//=================================================
				game->PlacePlayersRandom();
				StateManager::getInstance().push(game);
			}
		}
	}


	button.render(window);
}

void Menu::render(sf::RenderWindow* window)
{
}
