#include "Game.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include "DraftPhase.h"
#include "AttackPhase.h"
#include "FortifyPhase.h"

void Game::shuffle(std::vector<Territory*>& list)
{
	srand((unsigned int)time(NULL));
	unsigned int sizeOfList = list.size();
	for (unsigned int i = 0; i < sizeOfList; i++)
	{
		int newPos = rand() % sizeOfList;
		Territory* tmp = list[i];
		list[i] = list[newPos];
		list[newPos] = tmp;
	}
}

void Game::highlightPlayer(int index)
{
	for (unsigned int i = 0; i < m_playerButtons.size(); i++)
	{
		m_playerButtons[i]->setOutline(0, sf::Color::White);
	}
	sf::Color* col = &m_players[index]->GetColor();
	m_playerButtons[index]->setOutline(5, sf::Color::Color(255 - col->r, 255 - col->g, 255 - col->b));
}

bool Game::checkAllSelCardsDiff()
{
	bool res = true;
	for (int i = 0; i < m_selectedCards.size(); i++)
	{
		if (m_selectedCards[i]->type != Card::ArmyType::Joker)
		{
			for (int j = i + 1; j < m_selectedCards.size(); j++)
			{
				if (m_selectedCards[i]->type == m_selectedCards[j]->type && m_selectedCards[j]->type != Card::ArmyType::Joker)
				{
					res = false;
					break;
				}
			}
		}
	}
	return res;
}

bool Game::checkAllSelCardsSame()
{
	bool res = true;
	for (int i = 0; i < m_selectedCards.size(); i++)
	{
		if (m_selectedCards[i]->type != Card::ArmyType::Joker)
		{
			for (int j = i + 1; j < m_selectedCards.size(); j++)
			{
				if (m_selectedCards[i]->type != m_selectedCards[j]->type && m_selectedCards[j]->type != Card::ArmyType::Joker)
				{
					res = false;
					break;
				}
			}
		}
	}
	return res;
}

unsigned int Game::getUnitBonus(unsigned int index)
{
	switch (m_selectedCards[index]->type)
	{
	case Card::ArmyType::Infantry:
		return 4;
		break;
	case Card::ArmyType::Cavalry:
		return 6;
		break;
	case Card::ArmyType::Artillery:
		return 8;
		break;
	}
	return getUnitBonus(index + 1);
}

void Game::sortCardButtons(sf::RenderWindow* window)
{
	for (int i = 0; i < m_players[m_playerTurn]->GetNrOfOwnedCards(); i++)
	{
		Card* tmp = m_players[m_playerTurn]->GetCard(i);
		float xCoord = window->getSize().x;
		xCoord -= (3 + tmp->rect.getLocalBounds().width) * (i + 1);
		tmp->setPos(sf::Vector2f(xCoord, 700));
	}
}

Game::Game(const sf::Font& font, const char* pathToMap): m_font(font), m_selected(nullptr), m_playerTurn(0), m_firstDraft(true), m_tradePossible(false)
{
	if (m_mapTex.loadFromFile("../assets/map.png") && m_redScaleData.loadFromFile("../assets/mapRedScaleV2.png"))
	{
		m_mapSprite.setTexture(m_mapTex, true);
	}

	m_artTex.loadFromFile("../assets/artillery.png");
	m_cavTex.loadFromFile("../assets/cavalry.png");
	m_infTex.loadFromFile("../assets/infantry.png");
	m_jkrTex.loadFromFile("../assets/joker.png");

	LoadTerritories(pathToMap);

	m_btnNextPhase.setFont(&m_font);
	m_btnNextPhase.setPosition(sf::Vector2f(1000, 620));
	m_btnNextPhase.setString("Start Game");

	m_btnTradeCards.setFont(&m_font);
	m_btnTradeCards.setPosition(sf::Vector2f(1300, 620));
	m_btnTradeCards.setString("Trade Cards");

	//=============TESTING=============================
	AddPlayer("Jacob Andersson", sf::Color::Red);
	AddPlayer("Leo Wikström", sf::Color::Green);
	AddPlayer("Tim Johansson", sf::Color::Color(100, 100, 250));
	AddPlayer("Anton Åsbrink", sf::Color::Yellow);
	PlacePlayersRandom();
	//=================================================

	for (int i = 0; i < m_players.size(); i++)
	{
		Button* btn = new Button;
		btn->setFont(&m_font);
		btn->setColor(m_players[i]->GetColor());
		btn->setString(m_players[i]->GetName());
		btn->setPosition(sf::Vector2f(1300, (btn->getSize().y + 10) * i));
		m_playerButtons.push_back(btn);
	}
	m_phase = nullptr;
}

Game::~Game()
{
	for (unsigned int i = 0; i < m_freeCards.size(); i++)
	{
		delete m_freeCards[i];
	}

	for (unsigned int i = 0; i < m_busyCards.size(); i++)
	{
		delete m_busyCards[i];
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

	for (unsigned int i = 0; i < m_playerButtons.size(); i++)
	{
		delete m_playerButtons[i];
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

				if (m_redScaleData.getSize().x > (unsigned int)pos.x && m_redScaleData.getSize().y > (unsigned int)pos.y)
				{
					sf::Color col = m_redScaleData.getPixel(pos.x, pos.y);
					if (m_selected)
					{
						m_selected->GetTroopCountToken()->Shape.setOutlineThickness(0.f);
					}
					if (col.a != 0)
					{
						unsigned int colVal = col.r;
						Territory* tmp = m_territoryMapping.at(colVal);
						m_selected = tmp;
						m_selected->GetTroopCountToken()->Shape.setOutlineColor(sf::Color::Color(255, 165, 50));
						m_selected->GetTroopCountToken()->Shape.setOutlineThickness(10.f);
						/* // USED FOR DEBUG
						unsigned int nr = 0;
						for (unsigned int i = 0; i < m_territories.size(); i++)
						{
							if (m_territories[i] == tmp)
							{
								nr = i;
								break;
							}
						}
						*/
						std::cout << tmp->GetName() /*<< ": " << nr */<< std::endl;
					}
					else
					{
						m_selected = nullptr;
					}
				}
					
				if(m_phase)
					m_phase->run(window);

				if (m_btnNextPhase.isClicked(pos))
				{
					if(m_selected)
						m_selected->GetTroopCountToken()->Shape.setOutlineThickness(0.f);
					m_selected = nullptr;
					if (m_firstDraft)
					{
						if (!m_phase)
						{
							m_playerTurn = m_territories.size() % m_players.size();
							m_phase = new DraftPhase(this, m_players[m_playerTurn], &m_font);
							((DraftPhase*)m_phase)->SetDeployAmount(1);
							highlightPlayer(m_playerTurn);
							m_btnNextPhase.setString("End Draft");
						}
						else
						{
							if (m_players.size() > m_playerTurn)
							{
								delete m_phase;

								if (m_playerTurn == m_players.size() - 1)
									m_playerTurn = 0;
								else
									m_playerTurn++;

								m_phase = new DraftPhase(this, m_players[m_playerTurn], &m_font);
								((DraftPhase*)m_phase)->SetDeployAmount(1);
								highlightPlayer(m_playerTurn);
								m_btnNextPhase.setString("End Draft");
								bool done = true;
								unsigned int limit = 50 - m_players.size() * 5;
								for (int i = 0; i < m_players.size(); i++)
								{
									if (m_players[i]->GetArmyCount() < limit)
										done = false;
								}

								if (done)
								{
									m_firstDraft = false;
									m_btnNextPhase.setString("Start turn");
									delete m_phase;
									m_phase = nullptr;
								}
							}
						}
					}
					else
					{
						if (!m_phase)
						{
							m_phase = new DraftPhase(this, m_players[m_playerTurn], &m_font);
							m_btnNextPhase.setString("Go to Attack phase");
						}
						else if (dynamic_cast<DraftPhase*>(m_phase))
						{
							if (m_players[m_playerTurn]->GetNrOfOwnedCards() < 5)
							{
								m_selectedCards.clear();
								delete m_phase;
								m_phase = new AttackPhase(this, m_players[m_playerTurn], &m_font);
								m_btnNextPhase.setString("Go to Fortify phase");
								sortCardButtons(window);
							}
						}
						else if (dynamic_cast<AttackPhase*>(m_phase))
						{
							delete m_phase;
							m_phase = new FortifyPhase(this, m_players[m_playerTurn], &m_font);
							m_btnNextPhase.setString("End turn");
							sortCardButtons(window);
						}
						else if (dynamic_cast<FortifyPhase*>(m_phase))
						{
							delete m_phase;
							m_playerTurn++;
							if (m_playerTurn >= m_players.size())
							{
								m_playerTurn = 0;
							}
							m_phase = nullptr;
							m_btnNextPhase.setString("Start turn");
							highlightPlayer(m_playerTurn);
						}
					}
				}

				if (dynamic_cast<DraftPhase*>(m_phase))
				{
					if (m_tradePossible)
					{
						if (m_btnTradeCards.isClicked(pos))
						{
							DraftPhase* ph = dynamic_cast<DraftPhase*>(m_phase);
							bool allDiff = checkAllSelCardsDiff();
							bool allEq = checkAllSelCardsSame();

							if (allDiff)
							{
								ph->AddDeployAmount(10);
							}
							else if (allEq)
							{
								ph->AddDeployAmount(getUnitBonus(0));
							}

							for (unsigned int i = 0; i < m_selectedCards.size(); i++)
							{
								if (m_selectedCards[i]->territory->GetOwner() == m_players[m_playerTurn])
								{
									m_selectedCards[i]->territory->SetArmyCount(m_selectedCards[i]->territory->GetArmyCount() + 2);
									break;
								}
								m_players[m_playerTurn]->RemoveCard(m_selectedCards[i]);
							}

							m_selectedCards.clear();
						}
					}
					bool selected = false;
					for (int i = 0; i < m_players[m_playerTurn]->GetNrOfOwnedCards(); i++)
					{
						Card* tmp = m_players[m_playerTurn]->GetCard(i);

						if (tmp->rect.getGlobalBounds().contains(pos.x, pos.y))
						{
							m_selectedCards.push_back(tmp);
							tmp->rect.setOutlineColor(sf::Color::Blue);
							tmp->rect.setOutlineThickness(3.f);
							selected = true;
						}
						if (m_selectedCards.size() == 3)
						{
							bool allDiff = checkAllSelCardsDiff();
							bool allEq = checkAllSelCardsSame();

							if (allEq || allDiff)
								m_tradePossible = true;
							else
								m_tradePossible = false;
						}
						else
							m_tradePossible = false;
					}
					if (!selected)
					{
						for (int i = 0; i < m_selectedCards.size(); i++)
						{
							m_selectedCards[i]->rect.setOutlineThickness(0.f);
						}
						m_selectedCards.clear();
						m_tradePossible = false;
					}
				}
			}
	}
}

void Game::render(sf::RenderWindow* window)
{
	window->draw(m_mapSprite);
	for (unsigned int i = 0; i < m_troopCounts.size(); i++)
	{
		window->draw(m_troopCounts[i]->Shape);
		window->draw(m_troopCounts[i]->Txt);
	}

	for (unsigned int i = 0; i < m_playerButtons.size(); i++)
	{
		m_playerButtons[i]->render(window);
	}

	m_btnNextPhase.render(window);

	for (unsigned int i = 0; i < m_players[m_playerTurn]->GetNrOfOwnedCards(); i++)
	{
		Card* card = m_players[m_playerTurn]->GetCard(i);
		card->render(window);
	}

	if (m_tradePossible)
		m_btnTradeCards.render(window);

	if(m_phase)
		m_phase->render(window);
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
					c->tex = &m_infTex;
					break;
				case 'c':
					c->type = Card::ArmyType::Cavalry;
					c->territory = m_territories[territory];
					c->tex = &m_cavTex;
					break;
				case 'a':
					c->type = Card::ArmyType::Artillery;
					c->territory = m_territories[territory];
					c->tex = &m_artTex;
					break;
				case 'j':
					c->type = Card::ArmyType::Joker;
					c->territory = nullptr;
					c->tex = &m_jkrTex;
					break;
				default:
					break;
				}

				if (c->tex)
				{
					c->texSprite.setTexture(*c->tex, true);
				}
				c->txt.setFont(m_font);
				if (c->territory)
					c->txt.setString(c->territory->GetName());
				else
					c->txt.setString("");
				c->txt.setFillColor(sf::Color::Black);
				c->txt.setScale(sf::Vector2f(0.4f, 0.4f));
				c->rect.setSize(sf::Vector2f(100, 100));
				c->rect.setFillColor(sf::Color::White);

				m_freeCards.push_back(c);
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
				tc->Shape.setPosition((float)x, (float)y);
				tc->Shape.setFillColor(sf::Color::White);
				tc->Txt.setPosition((float)x + 5.f, (float)y);
				tc->Territory->SetArmyCount(1);
				tc->Txt.setString(std::to_string(tc->Territory->GetArmyCount()));
				m_troopCounts.push_back(tc);
				m_territories[territory]->AddTroopCountToken(tc);
			}
			ss.str("");
			ss.clear();
		}
	}
}

void Game::PlacePlayersRandom()
{
	std::vector<Territory*> tmp = m_territories;
	shuffle(tmp);
	unsigned int perPlayer = m_territories.size() / m_players.size();
	for (unsigned int i = 0; i < m_players.size(); i++)
	{
		for (unsigned int j = i * perPlayer; j < perPlayer * (1 + i); j++)
		{
			m_players[i]->AddTerritory(tmp[j]);
			tmp[j]->SetOwner(m_players[i]);
			TroopCount* tc = tmp[j]->GetTroopCountToken();
			tc->Shape.setFillColor(m_players[i]->GetColor());
			tmp[j]->SetArmyCount(1);
		}
	}
	if (perPlayer * m_players.size() < m_territories.size())
	{
		int j = 0;
		for (unsigned int i = perPlayer * m_players.size(); i < m_territories.size(); i++)
		{
			m_players[j]->AddTerritory(tmp[i]);
			tmp[i]->SetOwner(m_players[j]);
			tmp[i]->GetTroopCountToken()->Shape.setFillColor(m_players[j++]->GetColor());
			tmp[i]->SetArmyCount(1);
		}
	}
}

Territory* Game::GetSelected() const
{
	return m_selected;
}

Continent* Game::GetContinent(unsigned int index) const
{
	return m_continents[index];
}

unsigned int Game::GetNrOfContinents() const
{
	return m_continents.size();
}

void Game::GiveRandomCard(Player* player)
{
	unsigned int cardIndex = rand() % m_freeCards.size();
	player->AddCard(m_freeCards[cardIndex]);
	m_busyCards.push_back(m_freeCards[cardIndex]);
	m_freeCards.erase(m_freeCards.begin() + cardIndex);
}

void Game::PlayerDefeated(Player* victor, Player* player)
{
	for (auto iter = m_players.begin(); iter != m_players.end(); iter++)
	{
		if (player == *iter)
		{
			unsigned int pos = iter - m_players.begin();
			m_playerButtons[pos]->setColor(sf::Color::Color(100, 100, 100));
			unsigned int nrOfCards = player->GetNrOfOwnedCards();
			for (unsigned int i = 0; i < nrOfCards; i++)
			{
				victor->AddCard(player->GetCard(i));
			}
			for (unsigned int i = 0; i < nrOfCards; i++)
			{
				player->RemoveCard((unsigned int)0);
			}
			m_defeatedPlayers.push_back(player);
			m_players.erase(iter);
			break;
		}
	}
}

void Card::render(sf::RenderWindow* window)
{
	window->draw(rect);
	window->draw(txt);
	window->draw(texSprite);
}

void Card::setPos(sf::Vector2f pos)
{
	rect.setPosition(pos);
	if(territory)
		texSprite.setPosition(pos + sf::Vector2f(rect.getLocalBounds().width/2 - texSprite.getLocalBounds().width/2,50));
	else
		texSprite.setPosition(pos + sf::Vector2f(rect.getLocalBounds().width / 2 - texSprite.getLocalBounds().width / 2, 2));
	txt.setPosition(pos + sf::Vector2f(5, 10));

	if (txt.getLocalBounds().width > rect.getLocalBounds().width)
	{
		std::string tmp = txt.getString();
		unsigned int pos = tmp.find(' ');
		if(pos != -1)
			tmp[pos] = '\n';
		txt.setString(tmp);
	}
}
