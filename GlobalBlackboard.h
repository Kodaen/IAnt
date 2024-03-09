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
	std::map<Location*, Location*> _orders;

	// ------------ FUNCTIONS ------------ //
};