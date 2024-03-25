#pragma once

#include "Input.h"

class InputReinforcementIsValid : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputReinforcementIsValid(LocalBlackboard& _blackboard);
	~InputReinforcementIsValid();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};