#include "Menu.h"

Menu::Menu()
{
	font.loadFromFile("../arial.ttf");

	button.setFont(&font);
	button.setString("TEST");
	button.setPosition(sf::Vector2f(100, 100));
}

void Menu::run(sf::RenderWindow* window)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if (event.type == sf::Event::MouseButtonPressed) {
			if (button.isClicked(sf::Mouse::getPosition(*window)))
				button.setString("YES");
		}
	}


	button.render(window);
}
