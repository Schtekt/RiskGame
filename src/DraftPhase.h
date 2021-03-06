#ifndef DRAFTPHASE_H
#define DRAFTPHASE_H
#include "Phase.h"
#include "Button.h"
class DraftPhase : public Phase
{
private:
	unsigned int m_maxToDeploy;
	unsigned int m_toDeploy;

	Button m_lblShowDeploy;
	Button m_btnAddToDeploy;
	Button m_btnRemoveToDeploy;
	Button m_lblTotalToDeploy;
	Button m_btnConfirm;
	Button m_lblSelected;

	Territory* m_selectedTerritory;
	unsigned int calcToDeploy();
public:
	DraftPhase(Game* game, Player* player, sf::Font* font);
	~DraftPhase();
	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
	void SetDeployAmount(unsigned int toDeploy);
	void AddDeployAmount(unsigned int toAdd);
};

#endif