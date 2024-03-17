#pragma once
#include <string>
#include <fstream> 

#include "Location.h"
#include "Astar/Graph.h"
#include "Bug.h"
#define NBR_OF_TEAM 6

using namespace Astar;
using namespace std;

/// <summary>
/// Map system is the link between the Astar graph and the game, it will give a lot of user friendly functions to use path finding more easily
/// It also fills other duties relative to the map, such as finding and identying ant hills, returning the most probable ant hill for a given ant, 
/// and giving desirable destinations for exploration.
/// </summary>
class MapSystem
{
private:
	Astar::Graph<Location> _mapGraph;
	//Equivalent of a 2D array filled with false,  each cell's status will be accessible by typing isCellWalkable[row][col]
	std::vector<std::vector<bool>> _isCellWalkable;
	Location _antHills[NBR_OF_TEAM]; //The location of the anthill for each team, with the index being the default id of each hills in the map
	//The location of the anthill that we do not know the team of yet
	std::vector<Location> _unknowAnthills; 
	//The location of the anthill that we have found the team id for, team id is decided at runtime and depends on the order in which we meet the teams
	std::map<int, Location> _knowAnthills; 

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

	//Returns the ant closest to a given point, if there are multiple ants at the same distance, the first one found will be returned in priority
	std::vector<Location> getCloseEnoughAnts(const std::vector<Location>& ants, Location point, int maxDistance, int maxAntsNumber);

	//Each time our ants see an anthill, we register it in the map system to identify all the anthills as fast as possible
	void registerAnthillsSighting(int team, Location antHill)
	{
		//If we have no unknow anthills left, we can ignore this sighting
		if (_unknowAnthills.empty()) return;
		_knowAnthills[team] = antHill;
		_unknowAnthills.erase(std::remove(_unknowAnthills.begin(), _unknowAnthills.end(), antHill), _unknowAnthills.end());
		//If we have only one anthill left, we can register it as the anthill of the team
		if (_unknowAnthills.size() == 1)
		{
			_knowAnthills[team] = _unknowAnthills[0];
			_unknowAnthills.clear();
			return;
		}
	}

	//Return the most probable anthill for a given ant and team
	Location getMostProbableAnthill(Location ant, int team)
	{
		//If we have found the anthill of the ant's team, we return it
		if (_knowAnthills.find(team) != _knowAnthills.end()) return _knowAnthills[team];
		//If we have not found the anthill of the ant's team, we return the closest anthill
		auto closestAnthills = _mapGraph.findDataOfNodesBetween(ant, 0, INT_MAX, true, _unknowAnthills, true, 1);
		if (closestAnthills.empty()) 
		{
			_bug << "FATAL ERROR: big error in \"getMostProbableAnthill\", the anthill wasn't known, we've checked the closest ant hill on the entire map size, and we found nothing, very weird";
			return ant;
		}
	}
#if DEBUG
	void printMap();
#endif
};

