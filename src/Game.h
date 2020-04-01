#ifndef GAME_H
#define GAME_H
#include "Player.h"
class Game
{
private:
	std::vector<Player*> m_players;
	std::vector<Area*> m_areas;
public:
	Game();
	virtual ~Game();
	void AddPlayer(const std::string& name);
	void LoadAreas(const char* path);
};

#endif