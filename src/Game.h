#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "StateManager.h"
#include <unordered_map>
#include "Button.h"
class Phase;
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

	sf::Text txt;
	sf::Texture* tex;
	sf::Sprite texSprite;
	sf::RectangleShape rect;

	void render(sf::RenderWindow* window);
	void setPos(sf::Vector2f pos);
};

struct Continent
{
public:
	std::string Name;
	std::vector<Territory*> Territories;
	unsigned int BonusScore = 0;
};

class Game : public State
{
private:
	std::vector<Player*> m_players;
	std::vector<Territory*> m_territories;
	std::vector<Continent*> m_continents;
	std::vector<Card*> m_freeCards;
	std::vector<Card*> m_busyCards;
	std::map<unsigned int, Territory*> m_territoryMapping;
	std::vector<TroopCount*> m_troopCounts;

	Territory* m_selected;
	sf::Image m_redScaleData;
	sf::Texture m_mapTex;
	sf::Sprite m_mapSprite;
	sf::Texture m_artTex;
	sf::Texture m_cavTex;
	sf::Texture m_infTex;
	sf::Texture m_jkrTex;
	sf::Font m_font;
	Phase* m_phase;
	Button m_btnNextPhase;
	std::vector<Button*> m_playerButtons;
	unsigned int m_playerTurn;
	bool m_firstDraft;

	void shuffle(std::vector<Territory*>& list);
	void highlightPlayer(int index);
public:
	Game(const sf::Font &font, const char* pathToMap);
	virtual ~Game();

	void run(sf::RenderWindow* window);
	void render(sf::RenderWindow* window);

	void AddPlayer(const std::string& name, const sf::Color& color);
	
	void LoadTerritories(const char* path);
	void PlacePlayersRandom();
	Territory* GetSelected() const;
	Continent* GetContinent(unsigned int index) const;
	unsigned int GetNrOfContinents() const;
	void GiveRandomCard(Player* player);
};

#endif