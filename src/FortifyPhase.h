#ifndef FORTIFYPHASE_H
#define FORTIFYPHASE_H
#include "Phase.h"
class FortifyPhase : public Phase
{
private:
public:
	FortifyPhase(Game* game, Player* player);
	void run(sf::RenderWindow* window, sf::Event event);
	void render(sf::RenderWindow* window);
};
#endif