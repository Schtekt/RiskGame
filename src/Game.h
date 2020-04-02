#ifndef GAME_H
#define GAME_H
#include "Player.h"

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

struct Continent
{
public:
	std::string Name;
	std::vector<Territory*> Territories;
	unsigned int BonusScore;
};

class Game
{
private:
	std::vector<Player*> m_players;
	std::vector<Territory*> m_territories;
	std::vector<Continent*> m_continents;
	std::vector<Card*> m_cards;
public:
	Game();
	virtual ~Game();
	void AddPlayer(const std::string& name);
	void LoadTerritories(const char* path);
};

#endif