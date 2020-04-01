#ifndef AREA_H
#define AREA_H
#include <vector>
#include <string>
class Area
{
private:
	std::string m_name;
	unsigned int m_armyCount;
	std::vector<Area*> m_neighbours;
public:
	Area(const std::string& name);
	
	void AddNeighbour(Area* area);
	void SetArmyCount(unsigned int count);

	std::string GetName() const;
	unsigned int GetArmyCount() const;
	unsigned int NrOfNeighbours() const;
	Area* GetNeighbour(int index) const;
};


#endif