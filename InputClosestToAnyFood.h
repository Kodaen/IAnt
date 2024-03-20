#pragma once

#include "Input.h"

class InputClosestToFood : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputClosestToFood(LocalBlackboard& _blackboard);
	~InputClosestToFood();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};