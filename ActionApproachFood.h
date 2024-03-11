#pragma once

#include "Action.h"

class ActionApproachFood : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionApproachFood(LocalBlackboard& _blackboard);
	~ActionApproachFood();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

