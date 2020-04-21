#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "Button.h"
#include "Game.h"
#include "Network.h"
#include "Player.h"
#include <iostream>

#define PLAYER_NAME_HOST 0
#define PLAYER_NAME 1

class Menu : public State {
private:
	class Div {
	public:
		std::vector<Button> buttons;
		sf::Vector2f origin;

		bool active = false;

		void addButton(Button btn) {
			btn.setPosition(btn.getPos() + origin);
			buttons.push_back(btn);
		}

		void render(sf::RenderWindow *window) {
			for (int i = 0; i < buttons.size(); i++)
				buttons.at(i).render(window);
		}
	};

	sf::Font font;

	Div start;
	Button gameBtn;
	Button hostBtn;
	Button joinBtn;
	Button name;

	Div lobby;
	sf::Text ip;
	std::vector<sf::Text> players;
	Button addBtn;

	Div join;
	Button connectBtn;
	Button ipConnect;
	std::string inputStr;

	Network network;

public:
	Menu();

	void run(sf::RenderWindow *window);
	void render(sf::RenderWindow* window);
};

#endif