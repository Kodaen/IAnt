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
	int _colSize=0;
	int _rowSize=0;
	Bug _bug;

	void loadMapFromFile(std::ifstream mapFile);

public:
	MapSystem();
	~MapSystem();
	void setup();
	Astar::PathData<Location> findPath(Location from, Location to);

	//Return the shortest manhattan distance taking into account the map wrapping
	float getManhattanDistance(Location from, Location to) {
		int d1 = abs(from._row - to._row),
			d2 = abs(from._col - to._col),
			dr = min(d1, _rowSize - d1),
			dc = min(d2, _colSize - d2);
		return dr+dc;
	}
};

