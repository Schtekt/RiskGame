#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Risk Game");

	Menu menu;
    Game game;

    game.LoadAreas("../Territories.txt");

    while (window.isOpen())
    {

        window.clear();

		menu.run(&window);

        window.display();
    }

    return 0;
}