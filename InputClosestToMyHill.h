#pragma once

#include "Input.h"

class InputClosestToMyHill : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputClosestToMyHill(LocalBlackboard& _blackboard);
	~InputClosestToMyHill();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};