#ifndef PLAYER_H
#define PLAYER_H
#include "Territory.h"

class Player
{
private:
	std::string m_name;
	std::vector<Territory*> m_ownedTerritories;
	sf::Color m_color;
public:
	Player(const std::string& name, const sf::Color& color);

	void AddTerritory(Territory* area);
	void RemoveArea(Territory* area);
	
	std::string GetName() const;
	sf::Color GetColor() const;
};

#endif