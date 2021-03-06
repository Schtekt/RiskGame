#include "Territory.h"
#include "Player.h"
Territory::Territory(const std::string& name): m_owner(nullptr), m_troopCountToken(nullptr)
{
	m_name = name;
	m_armyCount = 0;
}
void Territory::AddNeighbour(Territory* area)
{
	m_neighbours.push_back(area);
}

void Territory::SetArmyCount(unsigned int count)
{
	m_armyCount = count;
	if(m_troopCountToken)
		m_troopCountToken->Txt.setString(std::to_string(m_armyCount));
}

void Territory::AddTroopCountToken(TroopCount* tc)
{
	m_troopCountToken = tc;
}

void Territory::SetOwner(Player* player)
{
	m_owner = player;
}

TroopCount* Territory::GetTroopCountToken()
{
	return m_troopCountToken;
}

std::string Territory::GetName() const
{
	return m_name;
}

unsigned int Territory::GetArmyCount() const
{
	return m_armyCount;
}

unsigned int Territory::NrOfNeighbours() const
{
	return m_neighbours.size();
}

Territory* Territory::GetNeighbour(int index) const
{
	return m_neighbours[index];
}

Player* Territory::GetOwner() const
{
	return m_owner;
}

