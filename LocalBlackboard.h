#pragma once

#include <vector>

#include "Location.h"
#include "NearbyFoodAnts.h"
#include "MapSystem.h"

class LocalBlackboard
{
	// ---------- CSTR & DSTR ---------- //
public :
	LocalBlackboard();
	~LocalBlackboard();


	// ------------ ATTRIBUTES ------------ //
public:
	// ant of which the behavior tree belongs to
	Location* p_ant;
	std::vector<NearbyFoodAnts> _nearbyFood;
	Location _targetFood;
	Location _targetEnemy;
	MapSystem::SentinelPoint _targetSentinelPoint;
	int _targetSentinelLastVisitedCache;

	Location _nextLocation;

	// ------------ FUNCTIONS ------------ //
};

