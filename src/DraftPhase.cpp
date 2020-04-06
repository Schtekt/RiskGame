#include "DraftPhase.h"

DraftPhase::DraftPhase(Game* game, Player* player, sf::Font* font): Phase(game, player, font), m_selectedTerritory(nullptr)
{
	m_toDeploy = std::max((unsigned int)3, player->GetNrOfTerritories() / 3);
	// TODO, add check for continents.

	m_btnAddToDeploy.setFont(font);
	m_btnRemoveToDeploy.setFont(font);
	m_lblShowDeploy.setFont(font);
	m_lblTotalToDeploy.setFont(font);

	m_btnAddToDeploy.setString(">");
	m_btnRemoveToDeploy.setString("<");
	m_lblShowDeploy.setString("");
	m_lblTotalToDeploy.setString(std::to_string(m_toDeploy));

	m_btnRemoveToDeploy.setPosition(sf::Vector2f(5, 600));
	m_lblShowDeploy.setPosition(sf::Vector2f(m_btnRemoveToDeploy.getSize().x + m_btnRemoveToDeploy.getPos().x + 5, 600));
	m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, 600));
}

void DraftPhase::run(sf::RenderWindow* window, sf::Event* event)
{
	if (m_game->GetSelected())
	{
		if (m_game->GetSelected()->GetOwner() == m_currPlayer)
		{
			m_lblShowDeploy.setString(std::to_string(m_toDeploy));
			m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, 600));
		}
		else
		{
			m_lblShowDeploy.setString("");
			m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, 600));
		}
	}
	else
	{
		m_lblShowDeploy.setString("");
		m_btnAddToDeploy.setPosition(sf::Vector2f(m_lblShowDeploy.getPos().x + m_lblShowDeploy.getSize().x + 5, 600));
	}
	while (window->pollEvent(*event))
	{
		if ((*event).type == sf::Event::Closed)
			window->close();
		if ((*event).type == sf::Event::MouseButtonPressed)
		{
		}
		if ((*event).type == sf::Event::KeyPressed)
		{
		}
	}
}

void DraftPhase::render(sf::RenderWindow* window)
{
	m_btnAddToDeploy.render(window);
	m_btnRemoveToDeploy.render(window);
	m_lblShowDeploy.render(window);
}
