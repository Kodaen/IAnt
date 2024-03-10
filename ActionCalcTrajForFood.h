#pragma once

#include "Action.h"

class ActionCalcTrajForFood : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionCalcTrajForFood(LocalBlackboard& _blackboard);
	~ActionCalcTrajForFood();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

