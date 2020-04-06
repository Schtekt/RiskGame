#ifndef FORTIFYPHASE_H
#define FORTIFYPHASE_H
#include "Phase.h"
class FortifyPhase : public Phase
{
private:
	Territory* m_selected;
	Territory* m_target;
	unsigned int m_nrToMove;
	bool m_hasMoved;

	Button m_btnAdd;
	Button m_btnDecrease;
	Button m_lblNumber;
	Button m_btnConfirm;

	Button m_lblSelected;
	Button m_lblTarget;
public:
	FortifyPhase(Game* game, Player* player, sf::Font* font);
	~FortifyPhase();
	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);
};
#endif