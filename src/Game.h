#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "StateManager.h"
#include <unordered_map>

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

struct TroopCount
{
public:
	TroopCount(const sf::Font& font, unsigned int shapeSize = 10): Shape(shapeSize), Territory(nullptr)
	{
		Txt.setFont(font);
		Txt.setFillColor(sf::Color::Black);
		Txt.setCharacterSize(15);
	};
	Territory* Territory;
	sf::CircleShape Shape;
	sf::Text Txt;
};

class Game : public State
{
private:
	std::vector<Player*> m_players;
	std::vector<Territory*> m_territories;
	std::vector<Continent*> m_continents;
	std::vector<Card*> m_cards;
	std::map<unsigned int, Territory*> m_territoryMapping;
	std::vector<TroopCount*> m_troopCounts;

	sf::Image m_redScaleData;
	sf::Texture m_tex;
	sf::Sprite m_sprite;
	sf::Font m_font;
public:
	Game(const sf::Font &font);
	virtual ~Game();

	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);

	void AddPlayer(const std::string& name);
	void LoadTerritories(const char* path);
};

#endif