#pragma once

#include "Input.h"

class InputCalledForReinforcement : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputCalledForReinforcement(LocalBlackboard& _blackboard);
	~InputCalledForReinforcement();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};