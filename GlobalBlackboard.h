#pragma once

#include <map>

#include "State.h"
#include "NearbyFoodAnts.h"
#include "MapSystem.h"
#include "Reinforcement.h"

class GlobalBlackboard
{
	// ---------- SINGLETON PATTERN ---------- //
public:
	static GlobalBlackboard& singleton() {
		return _instance;
	}

private:
	GlobalBlackboard();

	static GlobalBlackboard _instance;


	// ------------ ATTRIBUTES ------------ //
public :
	State _state;

	/* Key : Location where an want wants to move, 
	Value : Position of the ant which wants to move. */
	std::map<Location*, Location*> _orders;

	std::vector<NearbyFoodAnts> _nearbyFoodAnts;
	/* 
	Key: Location of the sentinel point currently target by an ant.
	Value: Location of the ant,
	*/
	std::map<MapSystem::SentinelPoint,Location> _explorationCacheSPToAnt;
	std::map<Location, MapSystem::SentinelPoint> _explorationCacheAntToSP;

	std::vector<Reinforcement> _reinforcements;

	// ------------ FUNCTIONS ------------ //
public :
	// move to tile if tile is walkable and another ant doesn't already want to walk here
	bool doMoveDirection(const Location& antLoc, int direction);
	void standStill(const Location& antLoc);
	bool doMoveLocation(const Location& antLoc, const Location& destLoc);
	bool isLocationFree(const Location& destLoc);
	vector<Location> getAdjacentOccupiedLocations(const Location& loc);

	bool pushReinforcement(const Reinforcement& newReinforcement);

	inline bool LocationMapContainsKey(std::map<Location*, Location*>& locMap, const Location& key);

	void emptyExplorationCache() {
		_explorationCacheSPToAnt.clear();
		_explorationCacheAntToSP.clear();
	};

	bool isSentinelPointInExplorationCache(const MapSystem::SentinelPoint& sp) {
		return _explorationCacheSPToAnt.count(sp);
	};

	bool isAntInExplorationCache(const Location& ant) {
		return _explorationCacheAntToSP.count(ant);
	};
};