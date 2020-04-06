#ifndef ATTACKPHASE_H
#define ATTACKPHASE_H
#include "Phase.h"
class AttackPhase : public Phase
{
private:
	Territory* m_selected;
	Territory* m_target;
	unsigned int nrOfDice;

	Button m_lblNrOfDice;
	Button m_btnAddDice;
	Button m_btnRemoveDice;
	Button m_btnConfirm;

	Button m_lblSelected;
	Button m_lblTarget;
public:
	AttackPhase(Game* game, Player* player, sf::Font* font);
	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
};
#endif