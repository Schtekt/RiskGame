#include "Player.h"

Player::Player(const std::string& name)
{
	m_name = name;
}

void Player::AddArea(Area* area)
{
	m_ownedAreas.push_back(area);
}

void Player::RemoveArea(Area* area)
{
	auto owned = m_ownedAreas.begin();
	while (owned != m_ownedAreas.end())
	{
		if (*owned == area)
		{
			m_ownedAreas.erase(owned);
			break;
		}
	}
}

std::string Player::GetName() const
{
	return m_name;
}
