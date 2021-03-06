#ifndef TERRITORY_H
#define TERRITORY_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct TroopCount;
class Player;
class Territory
{
private:
	std::string m_name;
	unsigned int m_armyCount;
	std::vector<Territory*> m_neighbours;
	TroopCount* m_troopCountToken;
	Player* m_owner;
public:
	Territory(const std::string& name);
	
	void AddNeighbour(Territory* area);
	void SetArmyCount(unsigned int count);
	void AddTroopCountToken(TroopCount* tc);
	void SetOwner(Player* player);
	TroopCount* GetTroopCountToken();

	std::string GetName() const;
	unsigned int GetArmyCount() const;
	unsigned int NrOfNeighbours() const;
	Territory* GetNeighbour(int index) const;
	Player* GetOwner() const;
};

struct TroopCount
{
public:
	TroopCount(const sf::Font& font, unsigned int shapeSize = 10) : Shape((float)shapeSize), Territory(nullptr)
	{
		Txt.setFont(font);
		Txt.setFillColor(sf::Color::Black);
		Txt.setCharacterSize(15);
	};

	Territory* Territory;
	sf::CircleShape Shape;
	sf::Text Txt;
};

#endif