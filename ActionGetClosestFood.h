#pragma once

#include "Action.h"

class ActionGetClosestFood : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionGetClosestFood(LocalBlackboard& _blackboard);
	~ActionGetClosestFood();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

