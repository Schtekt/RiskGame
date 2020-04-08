#include "DraftPhase.h"

unsigned int DraftPhase::calcToDeploy()
{
	unsigned int res = std::max((unsigned int)3, m_currPlayer->GetNrOfTerritories() / 3);
	for (int i = 0; i < m_game->GetNrOfContinents(); i++)
	{
		Continent* cont = m_game->GetContinent(i);
		bool ownsContinent = true;
		for (int j = 0; j < cont->Territories.size(); j++)
		{
			if (cont->Territories[j]->GetOwner() != m_currPlayer)
			{
				ownsContinent = false;
				break;
			}
		}
		if (ownsContinent)
			res += cont->BonusScore;
	}
	return res;
}

DraftPhase::DraftPhase(Game* game, Player* player, sf::Font* font): Phase(game, player, font), m_selectedTerritory(nullptr)
{
	m_toDeploy = calcToDeploy();
	m_maxToDeploy = m_toDeploy;
	// TODO, add check for continents.

	m_btnAddToDeploy.setFont(font);
	m_btnRemoveToDeploy.setFont(font);
	m_lblShowDeploy.setFont(font);
	m_lblTotalToDeploy.setFont(font);
	m_btnConfirm.setFont(font);
	m_lblSelected.setFont(font);

	m_btnAddToDeploy.setString(">");
	m_btnRemoveToDeploy.setString("<");
	m_lblShowDeploy.setString("");
	m_lblTotalToDeploy.setString(std::to_string(m_toDeploy));
	m_btnConfirm.setString("Confirm");
	m_lblSelected.setString(" ");

	m_btnRemoveToDeploy.setPosition(sf::Vector2f(5, 620));
	m_lblShowDeploy.setPosition(sf::Vector2f(m_btnRemoveToDeploy.getSize().x + m_btnRemoveToDeploy.getPos().x + 5, m_btnRemoveToDeploy.getPos().y));
	m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, m_btnRemoveToDeploy.getPos().y));
	m_btnConfirm.setPosition(sf::Vector2f(m_btnRemoveToDeploy.getPos().x, m_btnRemoveToDeploy.getPos().y + m_btnRemoveToDeploy.getSize().y + 5));
	m_lblSelected.setPosition(sf::Vector2f(m_btnConfirm.getPos().x, m_btnConfirm.getPos().y + m_btnConfirm.getSize().y + 5));
}

DraftPhase::~DraftPhase()
{
	if (m_maxToDeploy > 0 && m_currPlayer->GetNrOfTerritories() > 0)
	{
		for (int i = 0; i < m_maxToDeploy; i++)
		{
			int pos = rand() % m_currPlayer->GetNrOfTerritories();
			m_currPlayer->GetTerritory(pos)->SetArmyCount(m_currPlayer->GetTerritory(pos)->GetArmyCount() + 1);
		}
	}
}

void DraftPhase::run(sf::RenderWindow* window)
{
	if (m_selectedTerritory)
	{
		if (m_btnAddToDeploy.isClicked(sf::Mouse::getPosition(*window)))
		{
			m_toDeploy++;
			if (m_toDeploy > m_maxToDeploy)
			{
				m_toDeploy = m_maxToDeploy;
			}
		}

		if (m_btnRemoveToDeploy.isClicked(sf::Mouse::getPosition(*window)))
		{
			if(m_toDeploy != 0)
				m_toDeploy--;
			if (m_toDeploy < 1)
				m_toDeploy = 1;
		}

		if (m_maxToDeploy == 0)
		{
			m_toDeploy = 0;
		}

		if (m_btnConfirm.isClicked(sf::Mouse::getPosition(*window)))
		{
			m_selectedTerritory->SetArmyCount(m_selectedTerritory->GetArmyCount() + m_toDeploy);
			m_maxToDeploy = m_maxToDeploy - m_toDeploy;
			m_toDeploy = m_maxToDeploy;
		}

	}

	Territory* tmp = m_game->GetSelected();
	if (tmp)
	{
		if (tmp->GetOwner() == m_currPlayer)
		{
			m_selectedTerritory = tmp;
			m_lblShowDeploy.setString(std::to_string(m_toDeploy));
			m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, m_lblShowDeploy.getPos().y));
			m_lblSelected.setString(m_selectedTerritory->GetName());
			m_lblSelected.setColor(m_selectedTerritory->GetOwner()->GetColor());
		}
		else
		{
			m_selectedTerritory = nullptr;
		}
	}
	else
	{
		m_selectedTerritory = nullptr;
	}
}

void DraftPhase::render(sf::RenderWindow* window)
{
	if (m_selectedTerritory)
	{
		m_btnAddToDeploy.render(window);
		m_btnRemoveToDeploy.render(window);
		m_lblShowDeploy.render(window);
		m_btnConfirm.render(window);
		m_lblSelected.render(window);
	}
}

void DraftPhase::SetDeployAmount(unsigned int toDeploy)
{
	m_maxToDeploy = toDeploy;
	m_toDeploy = toDeploy;
}

void DraftPhase::AddDeployAmount(unsigned int toAdd)
{
	m_maxToDeploy += toAdd;
	m_toDeploy = m_maxToDeploy;
	m_lblShowDeploy.setString(std::to_string(m_toDeploy));
	m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, m_lblShowDeploy.getPos().y));
}
