#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "Button.h"
#include "Game.h"

class Menu : public State {
private:
	sf::Font font;
	Button button;

public:
	Menu();

	void run(sf::RenderWindow *window);
	void render(sf::RenderWindow* window);
};

#endif