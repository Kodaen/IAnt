#pragma once

#include "Input.h"

class InputAllAntsInPosition : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputAllAntsInPosition(LocalBlackboard& _blackboard);
	~InputAllAntsInPosition();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};