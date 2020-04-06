#ifndef ATTACKPHASE_H
#define ATTACKPHASE_H
#include "Phase.h"
class AttackPhase : public Phase
{
private:
public:
	AttackPhase(Game* game, Player* player, sf::Font* font);
	void run(sf::RenderWindow* window, sf::Event event);
	void render(sf::RenderWindow* window);
};
#endif