#pragma once
#include "Input.h"
class InputIDieByGoingThere : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputIDieByGoingThere(LocalBlackboard& _blackboard);
	~InputIDieByGoingThere();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};