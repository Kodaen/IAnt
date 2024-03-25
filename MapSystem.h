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
	struct SentinelPoint
	{
		Location _location;
		int _lastVisit;
		SentinelPoint(Location loc, int lastVisit) : _location(loc), _lastVisit(lastVisit) {};
		SentinelPoint(Location loc) : _location(loc), _lastVisit(0) {};
	};

	Astar::Graph<Location> _mapGraph;
	//Equivalent of a 2D array filled with false,  each cell's status will be accessible by typing isCellWalkable[row][col]
	std::vector<std::vector<bool>> _isCellWalkable;
	std::vector<std::vector<Location>> _cellNodes;
	//The sentinels point are points that "grid" the map, points where explorer should go and from there, see around
	std::vector<SentinelPoint*> _sentinelsPoints;
	std::vector<Location> _sentinelPointsLocations;

	//Each cell on the map is linked to it's closest sentinel point,
	//walking on one of those cells will increase the lastVisit of the sentinel point
	std::vector<std::vector<SentinelPoint*>> _tiedSentinelPoint;

	//The location of the anthill that we do not know the team of yet
	std::vector<Location> _unknowAnthills; 
	//The location of the anthill that we have found the team id for, team id is decided at runtime and depends on the order in which we meet the teams
	std::map<int, Location> _knowAnthills; 

	//Caching system, store the path data between turns to avoid recalculating the same path with one cell of difference each turn
	std::map<Location, PathData<Location>> _pathDataCache;

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
		for(auto sentinel : _sentinelsPoints)
			delete sentinel;
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
	Location moveToward(Location from, Location to);

	static MapSystem* getInstance();

#pragma region  Distance
	//Return the shortest manhattan distance taking into account the map wrapping
	float getManhattanDistance(Location from, Location to) {
		int d1 = abs(from._row - to._row),
			d2 = abs(from._col - to._col),
			dr = min(d1, _rowSize - d1),
			dc = min(d2, _colSize - d2);

		return dr + dc;
	}

	//Return the shortest euclidian distance taking into account the map wrapping
	float squaredEuclidianDistance(Location from, Location to) {
		int d1 = abs(from._row - to._row),
			d2 = abs(from._col - to._col),
			dr = min(d1, _rowSize - d1),
			dc = min(d2, _colSize - d2);

		return dr * dr + dc * dc;
	}
#pragma endregion
	
	std::vector<Location> getCloseEnoughAnts(const std::vector<Location>& ants, Location point, int maxDistance, int maxAntsNumber);

	//Will trace circles bigger and bigger around the origin to find the closest ground and return it
	//The getDistanceBetween function of the graph couldn't be used because here, we're not looking in terms of graph in cost, but in terms of pure geographical distance
	Location getClosestGround(Location origin, int maxDistance, int forcedXDirection=0, int forcedYDirection=0);

	void computeSentinelsPoints(const int &viewDistance);

	//Each time our ants see an anthill, we register it in the map system to identify all the anthills as fast as possible
	void registerAnthillsSighting(int team, Location antHill);

	//Return the most probable anthill for a given ant and team
	Location getMostProbableAnthill(Location ant, int team);

	//Must be called for each ant on each turn
	void updateSentinelsPoint(Location ant, int turn);
	Location moveTowardClosestUnknownAnthill(Location ant);

	std::vector<SentinelPoint*> getSentinelPoints()
	{
		return _sentinelsPoints;
	}
	std::vector<Location> getSentinelPointsLocations()
	{
		return _sentinelPointsLocations;
	}

	Location getClosestCellInSubset(Location ant, std::vector<Location> subset, int maxDistance=16) 
	{
		if(subset.size()<=0)
			return NULL_LOCATION;
		auto closestCell = _mapGraph.findDataOfNodesBetween(ant, 0, maxDistance, true, subset, true, 1);
		if(closestCell.size()<=0)
			return NULL_LOCATION;
		return closestCell[0];
	}

	//Once a path is calculated, we store it in the cache.
	//We store it in two versions: one starting at the ant current location, and one starting at the ant next location if it's able to moves
	void putPathInCache(Location from, PathData<Location> pathData)
	{
		_pathDataCache[from] = pathData;
		auto nextPos= pathData._reversePath.back();
		pathData._cost -= 1;
		pathData._reversePath.pop_back();
		_pathDataCache[nextPos] = pathData;
	}
#if DEBUG
	void printMap();
	void printSentinelsMap();
	//Show what an ant would see, not very accurate, nor optimized
	std::vector<Location> getViewCircles(const Location& origin,const int& maxDistance);
	void printSentinelsViewMap();
#endif
};

