#include "FortifyPhase.h"

FortifyPhase::FortifyPhase(Game* game, Player* player, sf::Font* font): Phase(game, player, font), m_nrToMove(0), m_selected(nullptr), m_target(nullptr), m_hasMoved(false)
{
	m_btnAdd.setFont(font);
	m_btnDecrease.setFont(font);
	m_lblNumber.setFont(font);
	m_btnConfirm.setFont(font);
	m_lblSelected.setFont(font);
	m_lblTarget.setFont(font);

	m_btnAdd.setString(">");
	m_btnDecrease.setString("<");
	m_lblNumber.setString("");
	m_btnConfirm.setString("Confirm");
	m_lblSelected.setString(" ");
	m_lblTarget.setString(" ");

	m_btnDecrease.setPosition(sf::Vector2f(5, 620));
	m_lblNumber.setPosition(sf::Vector2f(m_btnDecrease.getSize().x + m_btnDecrease.getPos().x + 5, m_btnDecrease.getPos().y));
	m_btnAdd.setPosition(sf::Vector2f(m_lblNumber.getPos().x + m_lblNumber.getSize().x + 5, m_lblNumber.getPos().y));
	m_btnConfirm.setPosition(sf::Vector2f(m_btnDecrease.getPos().x, m_btnDecrease.getPos().y + m_btnDecrease.getSize().y + 5));

	m_lblSelected.setColor(m_currPlayer->GetColor());
	m_lblTarget.setColor(m_currPlayer->GetColor());

	m_lblSelected.setPosition(sf::Vector2f(m_btnConfirm.getPos().x, m_btnConfirm.getPos().y + m_btnConfirm.getSize().y + 5));
	m_lblTarget.setPosition(sf::Vector2f(m_lblSelected.getPos().x, m_lblSelected.getPos().y + m_lblSelected.getSize().y + 5));
}

FortifyPhase::~FortifyPhase()
{
}

void FortifyPhase::run(sf::RenderWindow* window)
{
	if (!m_hasMoved)
	{
		if (m_game->GetSelected())
		{
			if (m_game->GetSelected()->GetOwner() == m_currPlayer)
			{
				if (!m_selected && m_game->GetSelected()->GetArmyCount() > 1)
				{
					m_selected = m_game->GetSelected();
					m_lblSelected.setString(m_selected->GetName());
				}
				else if (m_selected && m_game->GetSelected() != m_selected)
				{
					for (int i = 0; i < m_selected->NrOfNeighbours(); i++)
					{
						if (m_selected->GetNeighbour(i) == m_game->GetSelected())
						{
							m_target = m_game->GetSelected();
							m_lblTarget.setString(m_target->GetName());
							m_nrToMove = m_selected->GetArmyCount() - 1;
							break;
						}
					}
				}
				else if(m_game->GetSelected()->GetArmyCount() > 1)
				{
					m_target = nullptr;
					m_selected = m_game->GetSelected();
					m_lblSelected.setString(m_selected->GetName());
				}
			}
			else
			{
				m_selected = nullptr;
				m_target = nullptr;
			}
		}
		else
		{
			m_selected = nullptr;
			m_target = nullptr;
		}

		if (m_selected && m_target)
		{
			if (m_btnAdd.isClicked(sf::Mouse::getPosition(*window)))
			{
				m_nrToMove++;

				if (m_nrToMove > m_selected->GetArmyCount() - 1)
				{
					m_nrToMove = m_selected->GetArmyCount() - 1;
				}
			}

			if (m_btnDecrease.isClicked(sf::Mouse::getPosition(*window)))
			{
				if (m_nrToMove < 1)
				{
					m_nrToMove = 1;
				}
			}

			if (m_btnConfirm.isClicked(sf::Mouse::getPosition(*window)))
			{
				m_target->SetArmyCount(m_target->GetArmyCount() + m_nrToMove);
				m_selected->SetArmyCount(m_selected->GetArmyCount() - m_nrToMove);

				m_target->GetTroopCountToken()->Txt.setString(std::to_string(m_target->GetArmyCount()));
				m_selected->GetTroopCountToken()->Txt.setString(std::to_string((m_selected->GetArmyCount())));
				m_hasMoved = !m_hasMoved;
				m_selected = nullptr;
				m_target = nullptr;

			}

			m_lblNumber.setString(std::to_string(m_nrToMove));
			m_btnAdd.setPosition(sf::Vector2f(m_lblNumber.getPos().x + m_lblNumber.getSize().x + 5, m_lblNumber.getPos().y));
		}
	}
}

void FortifyPhase::render(sf::RenderWindow* window)
{
	if (m_selected)
	{
		m_lblSelected.render(window);
		if (m_target)
		{
			m_lblTarget.render(window);
			m_btnAdd.render(window);
			m_lblNumber.render(window);
			m_btnDecrease.render(window);
			m_btnConfirm.render(window);
		}
	}
}
