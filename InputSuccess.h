#pragma once

#include "Input.h"

class InputSuccess : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputSuccess() {}
	~InputSuccess() {}

	// ------------ FUNCTIONS ------------ //
public :
	EStatus checkCondition() { return BH_SUCCESS; };
};