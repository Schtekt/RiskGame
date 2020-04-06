#ifndef PHASE_H
#define PHASE_H
#include "Game.h"
class Phase
{
protected:
	Game* m_game;
	Player* m_currPlayer;
	sf::Font* m_font;
public:
	Phase(Game* game, Player* player, sf::Font* font);
	virtual ~Phase();
	virtual void run(sf::RenderWindow* window) = 0;
	virtual void render(sf::RenderWindow* window) = 0;
};
#endif