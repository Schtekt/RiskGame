#include "Game.h"
#include <fstream>
#include <sstream>
Game::Game()
{
}

Game::~Game()
{
	for (int i = 0; i < m_areas.size(); i++)
	{
		delete m_areas[i];
	}

	for (int i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
	}
}

void Game::AddPlayer(const std::string& name)
{
	m_players.push_back(new Player(name));
}

void Game::LoadAreas(const char* path)
{
	std::fstream file;
	file.open(path);
	
	if(file.is_open())
	{
		std::stringstream ss;
		char junk;
		// making a cheeky assumption that no line will exceed 128 characters.
		char line[128];

		while (!file.eof())
		{
			file.getline(line, 128);
			ss << line;

			if (line[0] == 'a')
			{
				std::string name;
				name = ss.str();
				name.erase(name.begin());
				name.erase(name.begin());
				m_areas.push_back(new Area(name));
			}
			else if (line[0] == 'n')
			{
				int neighbour;
				int target;
				ss >> junk >> target >> neighbour;

				m_areas[target]->AddNeighbour(m_areas[neighbour]);
				// if an area has a neighbour, that neighbour has the original area as a neighbour as well...
				m_areas[neighbour]->AddNeighbour(m_areas[target]);
			}
			ss.str("");
			ss.clear();
		}
	}
}
