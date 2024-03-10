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
};