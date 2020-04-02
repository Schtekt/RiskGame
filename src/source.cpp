#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Game.h"
#include "StateManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Risk Game");

	Menu menu;
	StateManager::getInstance().push(&menu);

    while (window.isOpen())
    {

        window.clear();

		StateManager::getInstance().run(&window);
		StateManager::getInstance().render(&window);

        window.display();
    }

    return 0;
}