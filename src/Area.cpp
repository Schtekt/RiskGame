#include "Area.h"

Area::Area(const std::string& name)
{
	m_name = name;
}
void Area::AddNeighbour(Area* area)
{
	m_neighbours.push_back(area);
}

void Area::SetArmyCount(unsigned int count)
{
	m_armyCount = count;
}

std::string Area::GetName() const
{
	return m_name;
}

unsigned int Area::GetArmyCount() const
{
	return m_armyCount;
}

unsigned int Area::NrOfNeighbours() const
{
	return m_neighbours.size();
}

Area* Area::GetNeighbour(int index) const
{
	return m_neighbours[index];
}

