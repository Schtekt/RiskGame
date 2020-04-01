#include "Player.h"

Player::Player(const std::string& name)
{
	m_name = name;
}

void Player::AddArea(Territory* area)
{
	m_ownedTerritories.push_back(area);
}

void Player::RemoveArea(Territory* area)
{
	auto owned = m_ownedTerritories.begin();
	while (owned != m_ownedTerritories.end())
	{
		if (*owned == area)
		{
			m_ownedTerritories.erase(owned);
			break;
		}
	}
}

std::string Player::GetName() const
{
	return m_name;
}
