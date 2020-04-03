#include "Game.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

void Game::shuffle(std::vector<Territory*>& list)
{
	srand(time(NULL));
	unsigned int sizeOfList = list.size();
	for (int i = 0; i < sizeOfList; i++)
	{
		int newPos = rand() % sizeOfList;
		Territory* tmp = list[i];
		list[i] = list[newPos];
		list[newPos] = tmp;
	}
}

Game::Game(const sf::Font& font): m_font(font)
{
	if (m_tex.loadFromFile("../assets/map.png") && m_redScaleData.loadFromFile("../assets/mapRedScale.png"))
	{
		m_sprite.setTexture(m_tex, true);
	}
}

Game::~Game()
{
	for (unsigned int i = 0; i < m_cards.size(); i++)
	{
		delete m_cards[i];
	}

	for (unsigned int i = 0; i < m_continents.size(); i++)
	{
		delete m_continents[i];
	}

	for (unsigned int i = 0; i < m_territories.size(); i++)
	{
		delete m_territories[i];
	}

	for (unsigned int i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
	}
}

void Game::run(sf::RenderWindow* window)
{
	for (int i = 0; i < m_territories.size(); i++)
	{
		m_territories[i]->GetTroopCountToken()->Txt.setString(std::to_string(m_territories[i]->GetArmyCount()));
	}

	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if(event.type == sf::Event::MouseButtonPressed)
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				sf::Vector2i pos = sf::Mouse::getPosition(*window);
				sf::Color col = m_redScaleData.getPixel(pos.x, pos.y);
				if (col.a != 0)
				{
					unsigned int colVal = col.r;
					Territory* tmp = m_territoryMapping.at(colVal);
					std::cout << tmp->GetName() << std::endl;
				}
			}
	}
}

void Game::render(sf::RenderWindow* window)
{
	window->draw(m_sprite);
	for (int i = 0; i < m_troopCounts.size(); i++)
	{
		window->draw(m_troopCounts[i]->Shape);
		window->draw(m_troopCounts[i]->Txt);
	}
}

void Game::AddPlayer(const std::string& name, const sf::Color& color)
{
	m_players.push_back(new Player(name, color));
}

void Game::LoadTerritories(const char* path)
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

			if (line[0] == 't')
			{
				std::string name;
				
				ss >> junk;
				std::getline(ss, name);
				name.erase(name.begin());

				m_territories.push_back(new Territory(name));
			}
			else if (line[0] == 'n')
			{
				int neighbour;
				int target;
				ss >> junk >> target >> neighbour;

				m_territories[target]->AddNeighbour(m_territories[neighbour]);
				// if an area has a neighbour, that neighbour has the original area as a neighbour as well...
				m_territories[neighbour]->AddNeighbour(m_territories[target]);
			}
			else if (line[0] == 'c')
			{
				unsigned int bonus;
				std::string name;

				ss >> junk >> bonus;
				std::getline(ss, name);
				name.erase(name.begin());

				Continent* c = new Continent();
				c->Name = name;
				c->BonusScore = bonus;
				m_continents.push_back(c);
			}
			else if (line[0] == 'i')
			{
				unsigned int continent;
				unsigned int territory;
				ss >> junk >> continent >> territory;
				m_continents[continent]->Territories.push_back(m_territories[territory]);
			}
			else if (line[0] == 'b')
			{
				unsigned int territory;
				char type;

				ss >> junk >> territory >> type;

				Card* c = new Card;
				switch (type)
				{
				case 'i':
					c->type = Card::ArmyType::Infantry;
					c->territory = m_territories[territory];
					break;
				case 'c':
					c->type = Card::ArmyType::Cavalry;
					c->territory = m_territories[territory];
					break;
				case 'a':
					c->type = Card::ArmyType::Artillery;
					c->territory = m_territories[territory];
					break;
				case 'j':
					c->type = Card::ArmyType::Joker;
					c->territory = nullptr;
					break;
				default:
					break;
				}
				m_cards.push_back(c);
			}
			else if (line[0] == 'm')
			{
				unsigned int colVal;
				unsigned int ter;
				std::pair<unsigned int, Territory*> tmp;
				ss >> junk >> colVal >> ter;
				tmp.first = colVal;
				tmp.second = m_territories[ter];
				m_territoryMapping.insert(tmp);
			}
			else if (line[0] == 'p')
			{
				unsigned int territory;
				unsigned int x;
				unsigned int y;

				ss >> junk >> territory >> x >> y;

				TroopCount* tc = new TroopCount(m_font);
				tc->Territory = m_territories[territory];
				tc->Shape.setPosition(x, y);
				tc->Shape.setFillColor(sf::Color::White);
				tc->Txt.setPosition(x + 5, y);
				tc->Txt.setString(std::to_string(tc->Territory->GetArmyCount()));
				m_troopCounts.push_back(tc);
				m_territories[territory]->AddTroopCountToken(tc);
			}
			ss.str("");
			ss.clear();
		}

		for (int i = 0; i < m_troopCounts.size(); i++)
		{
			m_troopCounts[i]->Txt.setString(std::to_string(m_troopCounts[i]->Territory->GetArmyCount()));
		}
	}
}

void Game::PlacePlayersRandom()
{
	std::vector<Territory*> tmp = m_territories;
	shuffle(tmp);
	unsigned int perPlayer = m_territories.size() / m_players.size();
	for (int i = 0; i < m_players.size(); i++)
	{
		for (int j = i * perPlayer; j < perPlayer * (1 + i); j++)
		{
			m_players[i]->AddTerritory(tmp[j]);
			TroopCount* tc = tmp[j]->GetTroopCountToken();
			tc->Shape.setFillColor(m_players[i]->GetColor());
			tmp[j]->SetArmyCount(1);
		}
	}
	if (perPlayer * m_players.size() < m_territories.size())
	{
		int j = 0;
		for (int i = perPlayer * m_players.size(); i < m_territories.size(); i++)
		{
			m_players[j]->AddTerritory(tmp[i]);
			tmp[i]->GetTroopCountToken()->Shape.setFillColor(m_players[j++]->GetColor());
			tmp[i]->SetArmyCount(1);
		}
	}
}
