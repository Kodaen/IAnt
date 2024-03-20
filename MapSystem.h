#pragma once
#include <string>
#include <fstream> 

#include "Location.h"
#include "Astar/Graph.h"
#include "Bug.h"

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
	std::vector<std::vector<Location>> _cellNodes;
	//The sentinels point are points that "grid" the map, points where explorer should go and from there, see around
	std::vector<std::vector<Location>> _sentinelsPoints;

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
	
	std::vector<Location> getCloseEnoughAnts(std::vector<Location>& ants, Location point, int maxDistance, int maxAntsNumber);
	std::vector<Location> computeSentinelsPoints(int viewDistance) 
	{
		std::vector<Location> walls= {};
		std::vector<Location> ground= {};
		for (int row = 0; row < _rowSize; row++)
		{
			for (int col = 0; col < _colSize; col++)
			{
				if (!_isCellWalkable[row][col])
				{
					walls.push_back(Location(row, col));
				}
				else
				{
					ground.push_back(Location(row, col));
				}
			}
		}

		//We pick a random point on the ground to start the computation
		Location computedPoint= ground[rand() % ground.size()];

		int nbrOfSentilPoints = 0;
		std::vector<Location> _sentinelsPoints= {};
		while (nbrOfSentilPoints < 1)
		{
			auto closeWalls=_mapGraph.findDataOfNodesBetween(computedPoint, 1, viewDistance, true, walls, true, 1);
			//Far enough of all walls, can move on
			if (closeWalls.size() == 0)
			{
				_sentinelsPoints.push_back(computedPoint);
				nbrOfSentilPoints++;
				continue;
			}
			
			//Too close to a wall, should try to back away
			auto closestWall= closeWalls[0];
		}
	}

	//Each time our ants see an anthill, we register it in the map system to identify all the anthills as fast as possible
	void registerAnthillsSighting(int team, Location antHill);

	//Return the most probable anthill for a given ant and team
	Location getMostProbableAnthill(Location ant, int team);
#if DEBUG
	void printMap();
#endif
};

