#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "Button.h"

class Menu {
private:
	sf::Font font;

	Button button;
	sf::Event event;

public:
	Menu();

	void run(sf::RenderWindow *window);
};

#endif