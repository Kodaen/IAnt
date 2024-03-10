#pragma once

#include <map>

#include "State.h"
#include "NearbyFoodAnts.h"

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

	// ------------ FUNCTIONS ------------ //
public :
	// move to tile if tile is walkable and another ant doesn't already want to walk here
	bool doMoveDirection(const Location& antLoc, int direction);
	bool doMoveLocation(const Location& antLoc, const Location& destLoc);

	inline bool LocationMapContainsKey(std::map<Location*, Location*>& locMap, const Location& key);

};