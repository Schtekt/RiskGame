#include "AttackPhase.h"

AttackPhase::AttackPhase(Game* game, Player* player, sf::Font* font): Phase(game, player, font), m_selected(nullptr), m_target(nullptr),nrOfDice(3)
{
	m_btnAddDice.setFont(font);
	m_btnRemoveDice.setFont(font);
	m_btnConfirm.setFont(font);
	m_lblNrOfDice.setFont(font);
	m_lblSelected.setFont(font);
	m_lblTarget.setFont(font);

	m_btnAddDice.setString(">");
	m_btnRemoveDice.setString("<");
	m_btnConfirm.setString("Confirm");
	m_lblNrOfDice.setString(" ");
	m_lblSelected.setString("");
	m_lblTarget.setString("");

	m_btnRemoveDice.setPosition(sf::Vector2f(5, 620));
	m_lblNrOfDice.setPosition(sf::Vector2f(m_btnRemoveDice.getSize().x + m_btnRemoveDice.getPos().x + 5, m_btnRemoveDice.getPos().y));
	m_btnAddDice.setPosition(sf::Vector2f(m_lblNrOfDice.getPos().x + m_lblNrOfDice.getSize().x + 5, m_lblNrOfDice.getPos().y));
	m_btnConfirm.setPosition(sf::Vector2f(m_btnRemoveDice.getPos().x, m_btnRemoveDice.getPos().y + m_btnRemoveDice.getSize().y + 5));

	m_lblSelected.setPosition(sf::Vector2f(m_btnConfirm.getPos().x, m_btnConfirm.getPos().y + m_btnConfirm.getSize().y + 5));
	m_lblTarget.setPosition(sf::Vector2f(m_lblSelected.getPos().x, m_lblSelected.getPos().y + m_lblSelected.getSize().y + 5));

}

void AttackPhase::run(sf::RenderWindow* window)
{
	if (m_game->GetSelected())
	{
		if (!m_selected)
		{
			if (m_game->GetSelected()->GetOwner() == m_currPlayer)
				m_selected = m_game->GetSelected();
		}
		else if (m_game->GetSelected()->GetOwner() != m_currPlayer && m_game->GetSelected() != m_target)
		{
			for (unsigned int i = 0; i < m_selected->NrOfNeighbours(); i++)
			{
				if (m_game->GetSelected() == m_selected->GetNeighbour(i))
				{
					m_target = m_game->GetSelected();
					nrOfDice = std::min((unsigned int)3, m_selected->GetArmyCount() - 1);
					if (nrOfDice == 0)
					{
						m_selected = nullptr;
						m_target = nullptr;
					}
					break;
				}
			}
		}
		else if(m_game->GetSelected()->GetOwner() == m_currPlayer)
		{
			m_selected = m_game->GetSelected();
		}
	}
	else
	{
		m_selected = nullptr;
		m_target = nullptr;
	}

	if (m_selected)
	{
		m_lblSelected.setString(m_selected->GetName());
		m_lblSelected.setColor(m_selected->GetOwner()->GetColor());
		if (m_target)
		{
			m_lblTarget.setString(m_target->GetName());

			m_lblTarget.setColor(m_target->GetOwner()->GetColor());
		}
	}

	if (m_selected && m_target)
	{
		if (m_btnAddDice.isClicked(sf::Mouse::getPosition(*window)))
		{
			nrOfDice++;
			if (nrOfDice > m_selected->GetArmyCount() - 1)
			{
				nrOfDice = m_selected->GetArmyCount() - 1;
			}
			else if (nrOfDice > 3)
			{
				nrOfDice = 3;
			}
		}

		if (m_btnRemoveDice.isClicked(sf::Mouse::getPosition(*window)))
		{
			nrOfDice--;
			if (nrOfDice == 0)
			{
				nrOfDice = 1;
			}
		}

		m_lblNrOfDice.setString(std::to_string(nrOfDice));
		m_btnAddDice.setPosition(sf::Vector2f(m_lblNrOfDice.getPos().x + m_lblNrOfDice.getSize().x + 5, m_lblNrOfDice.getPos().y));
	}
}

void AttackPhase::render(sf::RenderWindow* window)
{
	if (m_selected && m_target)
	{
		m_btnAddDice.render(window);
		m_btnRemoveDice.render(window);
		m_btnConfirm.render(window);
		m_lblNrOfDice.render(window);
		m_lblSelected.render(window);
		m_lblTarget.render(window);
	}
}
