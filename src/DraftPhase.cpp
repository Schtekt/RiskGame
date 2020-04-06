#include "DraftPhase.h"

DraftPhase::DraftPhase(Game* game, Player* player, sf::Font* font): Phase(game, player, font), m_selectedTerritory(nullptr)
{
	m_toDeploy = std::max((unsigned int)3, player->GetNrOfTerritories() / 3);
	m_maxToDeploy = m_toDeploy;
	// TODO, add check for continents.

	m_btnAddToDeploy.setFont(font);
	m_btnRemoveToDeploy.setFont(font);
	m_lblShowDeploy.setFont(font);
	m_lblTotalToDeploy.setFont(font);
	m_btnConfirm.setFont(font);

	m_btnAddToDeploy.setString(">");
	m_btnRemoveToDeploy.setString("<");
	m_lblShowDeploy.setString("");
	m_lblTotalToDeploy.setString(std::to_string(m_toDeploy));
	m_btnConfirm.setString("Confirm");

	m_btnRemoveToDeploy.setPosition(sf::Vector2f(5, 620));
	m_lblShowDeploy.setPosition(sf::Vector2f(m_btnRemoveToDeploy.getSize().x + m_btnRemoveToDeploy.getPos().x + 5, m_btnRemoveToDeploy.getPos().y));
	m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, m_btnRemoveToDeploy.getPos().y));
	m_btnConfirm.setPosition(sf::Vector2f(m_btnRemoveToDeploy.getPos().x, m_btnRemoveToDeploy.getPos().y + m_btnRemoveToDeploy.getSize().y + 5));
}

DraftPhase::~DraftPhase()
{
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
	}
}
