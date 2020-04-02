#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include "State.h"
#include <vector>
#include <SFML/Graphics.hpp>

class StateManager {
private:
	StateManager() {}

	std::vector<State*> states;
public:
	static StateManager& getInstance();

	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);

	void push(State* state);
	void pop();
};
#endif // !STATEMANAGER_H
