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
	Bug _bug;

	void createGraphFromMapFile(std::ifstream mapFile);
public:
	MapSystem();
	~MapSystem();
	void setup();
	void getPath(Location from, Location to);
};

