#ifndef PLAYER_H
#define PLAYER_H
#include "Area.h"

class Player
{
private:
	std::string m_name;
	std::vector<Area*> m_ownedAreas;
public:
	Player(const std::string& name);

	void AddArea(Area* area);
	void RemoveArea(Area* area);
	
	std::string GetName() const;
};

#endif