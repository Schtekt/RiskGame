#include <SFML/Graphics.hpp>
#include "Menu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Risk Game");

	Menu menu;

    while (window.isOpen())
    {

        window.clear();

		menu.run(&window);

        window.display();
    }

    return 0;
}