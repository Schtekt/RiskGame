#ifndef FORTIFYPHASE_H
#define FORTIFYPHASE_H
#include "Phase.h"
class FortifyPhase : public Phase
{
private:
	
public:
	FortifyPhase(Game* game, Player* player, sf::Font* font);
	~FortifyPhase();
	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
};
#endif