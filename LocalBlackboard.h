#pragma once

#include <vector>

#include "Location.h"

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
	std::vector<Location> _nearbyFood;

	// ------------ FUNCTIONS ------------ //
};

