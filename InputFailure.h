#pragma once

#include "Input.h"

class InputFailure : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputFailure() {}
	~InputFailure() {}

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition() { return BH_FAILURE; };
};