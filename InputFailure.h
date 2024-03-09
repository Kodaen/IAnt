#pragma once

#include "Input.h"

class InputFailure : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputFailure(LocalBlackboard &_blackboard) : Input(_blackboard) {}
	~InputFailure() {}

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition() { return BH_FAILURE; };
};