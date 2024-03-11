#pragma once
#include <string>
#include <fstream> 

#include "Location.h"
#include "Astar/Graph.h"
#include "Bug.h"

using namespace Astar;
using namespace std;

class MapSystem
{
private:
	Astar::Graph<Location> _mapGraph;
	//Equivalent of a 2D array filled with false,  each cell's status will be accessible by typing isCellWalkable[row][col]
	std::vector<std::vector<bool>> _isCellWalkable;
	int _colSize = 0;
	int _rowSize = 0;
	Bug _bug;

	void loadMapFromFile(std::ifstream mapFile);

	MapSystem()
	{
		_bug.open("./mapSystemDebug.txt");
	}

	~MapSystem()
	{
		_bug.close();
	}

	static MapSystem *_instance;
public:
	MapSystem(MapSystem& other) = delete;
	void operator=(const MapSystem&) = delete;
	
	void setup();
	Astar::PathData<Location> findPath(Location from, Location to);
	float findCost(Location from, Location to)
	{
		return findPath(from, to)._cost;
	}
	//Return a location one step closer to the destination
	Location moveToward(Location from, Location to) 
	{
		auto path= findPath(from, to)._reversePath;
		return path.back();
	}

	static MapSystem* getInstance();

	//Return the shortest manhattan distance taking into account the map wrapping
	float getManhattanDistance(Location from, Location to) {
		int d1 = abs(from._row - to._row),
			d2 = abs(from._col - to._col),
			dr = min(d1, _rowSize - d1),
			dc = min(d2, _colSize - d2);
		return dr + dc;
	}

	std::vector<Location> getCloseEnoughAnts(std::vector<Location>& ants, Location point, int maxDistance, int maxAntsNumber)
	{
		auto closeAnts = _mapGraph.findDataOfNodesBetween(point, 0, maxDistance,true,ants,true,maxAntsNumber);
		ants = closeAnts;
		return ants;
	}

#if DEBUG
	void printMap();
#endif
};

