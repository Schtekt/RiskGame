#include "StateManager.h"

StateManager& StateManager::getInstance()
{
	static StateManager instance;
	return instance;
}

void StateManager::run(sf::RenderWindow* window)
{
	states.at(states.size()-1)->run(window);
}

void StateManager::render(sf::RenderWindow* window)
{
	states.at(states.size() - 1)->render(window);
}

void StateManager::push(State* state)
{
	states.push_back(state);
}

void StateManager::pop()
{
	states.pop_back();
}
