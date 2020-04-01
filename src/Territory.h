#ifndef TERRITORY_H
#define TERRITORY_H
#include <vector>
#include <string>
class Territory
{
private:
	std::string m_name;
	unsigned int m_armyCount;
	std::vector<Territory*> m_neighbours;
public:
	Territory(const std::string& name);
	
	void AddNeighbour(Territory* area);
	void SetArmyCount(unsigned int count);

	std::string GetName() const;
	unsigned int GetArmyCount() const;
	unsigned int NrOfNeighbours() const;
	Territory* GetNeighbour(int index) const;
};


#endif