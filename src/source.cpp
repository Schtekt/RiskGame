#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 800), "GO!");

	Game game;

    while (window.isOpen())
    {

        window.clear();

		game.run(window);
		game.render(window);

        window.display();
    }

    return 0;
}