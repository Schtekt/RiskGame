#ifndef PLAYER_H
#define PLAYER_H
#include "Territory.h"

class Player
{
private:
	std::string m_name;
	std::vector<Territory*> m_ownedTerritories;
public:
	Player(const std::string& name);

	void AddArea(Territory* area);
	void RemoveArea(Territory* area);
	
	std::string GetName() const;
};

#endif