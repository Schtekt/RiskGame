#include "Game.h"
#include <fstream>
#include <sstream>
#include <iostream>

Game::Game()
{
	if (m_tex.loadFromFile("../dependencies/map.png") && m_redScaleData.loadFromFile("../dependencies/mapRedScale.png"))
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
					//std::map<unsigned int, Territory*>::iterator it = m_territoryMapping.find(colVal);
					//Territory* tmp = it->second;
					std::cout << tmp->GetName() << std::endl;
				}
			}
	}
}

void Game::render(sf::RenderWindow* window)
{
	window->draw(m_sprite);
}

void Game::AddPlayer(const std::string& name)
{
	m_players.push_back(new Player(name));
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
			ss.str("");
			ss.clear();
		}
	}
}
