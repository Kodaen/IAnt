#pragma once

#include "Input.h"

class InputWeBothDie : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputWeBothDie(LocalBlackboard& _blackboard);
	~InputWeBothDie();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};