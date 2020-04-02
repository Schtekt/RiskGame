#ifndef STATE_H
#define STATE_H
#include <SFML/Graphics.hpp>

class State {
private:

protected:
	sf::Event event;
public:
	State() {}

	virtual void run(sf::RenderWindow* window) = 0;
	virtual void render(sf::RenderWindow* window) = 0;
};
#endif // !STATE_H
