#include "Player.h"

Player::Player(const std::string& name, const sf::Color& color): m_name(name), m_color(color)
{
}

void Player::AddTerritory(Territory* area)
{
	m_ownedTerritories.push_back(area);
}

void Player::RemoveOwnership(Territory* area)
{
	auto owned = m_ownedTerritories.begin();
	while (owned != m_ownedTerritories.end())
	{
		if (*owned == area)
		{
			m_ownedTerritories.erase(owned);
			break;
		}
		owned++;
	}
}

std::string Player::GetName() const
{
	return m_name;
}

sf::Color Player::GetColor() const
{
	return m_color;
}

unsigned int Player::GetNrOfTerritories() const
{
	return m_ownedTerritories.size();
}

Territory* Player::GetTerritory(unsigned int index) const
{
	return m_ownedTerritories[index];
}

unsigned int Player::GetArmyCount() const
{
	unsigned int res = 0;
	for (int i = 0; i < m_ownedTerritories.size(); i++)
	{
		res += m_ownedTerritories[i]->GetArmyCount();
	}
	return res;
}

void Player::AddCard(Card* card)
{
	m_ownedCards.push_back(card);
}

void Player::RemoveCard(unsigned int index)
{
	m_ownedCards.erase(m_ownedCards.begin() + index);
}

Card* Player::GetCard(unsigned int index)
{
	return m_ownedCards[index];
}

unsigned int Player::GetNrOfOwnedCards() const
{
	return m_ownedCards.size();
}
