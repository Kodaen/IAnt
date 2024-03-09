#pragma once

#include "Input.h"

class InputSuccess : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputSuccess(LocalBlackboard &_blackboard) : Input(_blackboard) {}
	~InputSuccess() {}

	// ------------ FUNCTIONS ------------ //
public :
	EStatus checkCondition() { return BH_SUCCESS; };
};