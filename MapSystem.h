#pragma once
#include <string>
#include <fstream> 

#include "Location.h"
#include "Graph.h"
#include "Bug.h"

using namespace Astar;
class MapSystem
{
private:
	Astar::Graph<Location> _mapGraph;
	int _width;
	int _height;
	Bug _bug;

	void loadMapFromFile(std::ifstream mapFile);
public:
	MapSystem();
	~MapSystem();
	void setup();
	Astar::PathData<Location> findPath(Location from, Location to);
	static float getManhattanDistance(Location from, Location to) {
		return abs(from._col - to._col) + abs(from._row - to._row);
	}
};

