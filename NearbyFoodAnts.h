#pragma once

#include <set>

#include "Location.h"

struct NearbyFoodAnts
{
	Location foodLoc;
	std::set<Location> nearbyAntsLoc;
	Location closestAntLoc;
	float closestDistance;
};