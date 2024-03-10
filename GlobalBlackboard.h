#pragma once

#include "State.h"
#include <map>

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

	/* Key: contains food,
	Value : Nearby ants to this food*/ 
	std::map<Location, std::vector<Location>> _nearbyFoodAnts;

	// ------------ FUNCTIONS ------------ //
};