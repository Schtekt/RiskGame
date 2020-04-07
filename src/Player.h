#ifndef PLAYER_H
#define PLAYER_H
#include "Territory.h"

struct Card
{
	enum class ArmyType
	{
		Infantry,
		Cavalry,
		Artillery,
		Joker
	};
	Territory* territory;
	ArmyType type;
};

class Player
{
private:
	std::string m_name;
	std::vector<Territory*> m_ownedTerritories;
	std::vector<Card*> m_ownedCards;
	sf::Color m_color;
public:
	Player(const std::string& name, const sf::Color& color);

	void AddTerritory(Territory* area);
	void RemoveOwnership(Territory* area);
	
	std::string GetName() const;
	sf::Color GetColor() const;
	unsigned int GetNrOfTerritories() const;
	Territory* GetTerritory(unsigned int index) const;
	unsigned int GetArmyCount() const;
	void AddCard(Card* card);
	void RemoveCard(unsigned int index);
	Card* GetCard(unsigned int index);
	unsigned int GetNrOfOwnedCards() const;
};

#endif