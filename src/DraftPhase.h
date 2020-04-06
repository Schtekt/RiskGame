#ifndef DRAFTPHASE_H
#define DRAFTPHASE_H
#include "Phase.h"
#include "Button.h"
struct Deployment
{
	unsigned int Count;
	Territory* territory;
};

class DraftPhase : public Phase
{
private:
	unsigned int m_toDeploy;
	std::vector<Deployment> m_deployed;

	Button m_lblShowDeploy;
	Button m_btnAddToDeploy;
	Button m_btnRemoveToDeploy;
	Button m_lblTotalToDeploy;

	Territory* m_selectedTerritory;
public:
	DraftPhase(Game* game, Player* player, sf::Font* font);
	void run(sf::RenderWindow* window, sf::Event* event);
	void render(sf::RenderWindow* window);
};

#endif