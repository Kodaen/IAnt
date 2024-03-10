#pragma once

#include "Input.h"

class InputCloseAnyFood : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputCloseAnyFood(LocalBlackboard& _blackboard);
	~InputCloseAnyFood();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};