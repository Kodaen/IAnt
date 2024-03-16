#pragma once

#include "Input.h"

class InputEnemyNear : public Input {
	// ------------ CSTR & DSTR ------------ //
public:
	InputEnemyNear(LocalBlackboard& _blackboard);
	~InputEnemyNear();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus checkCondition();
};