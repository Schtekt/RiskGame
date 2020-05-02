#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "GO!");

	Game game;
	sf::Event event;

    while (window.isOpen())
    {
		if (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}


        window.clear();

		game.render(window);

        window.display();
    }

    return 0;
}