#ifndef DRAFTPHASE_H
#define DRAFTPHASE_H
#include "Phase.h"

struct Deployment
{
	unsigned int Count;
	Territory* territory;
};

class DraftPhase : public Phase
{
private:
	unsigned int toDeploy;
	std::vector<Deployment> dep;
public:
	DraftPhase(Game* game, Player* player);
	void run(sf::RenderWindow* window, sf::Event event);
	void render(sf::RenderWindow* window);
};

#endif