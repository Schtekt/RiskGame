#ifndef ATTACKPHASE_H
#define ATTACKPHASE_H
#include "Phase.h"
class AttackPhase : public Phase
{
private:
	Territory* m_selected;
	Territory* m_target;
	unsigned int m_nrOfDice;
	bool m_occupy;

	Button m_lblNrOfDice;
	Button m_btnAddDice;
	Button m_btnRemoveDice;
	Button m_btnConfirm;

	Button m_lblSelected;
	Button m_lblTarget;
	void battle(Territory* attacker, Territory* defender, unsigned int nrOfAttackDice);
public:
	AttackPhase(Game* game, Player* player, sf::Font* font);
	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
};
#endif