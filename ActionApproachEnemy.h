#pragma once

#include "Action.h"

class ActionApproachEnemy : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionApproachEnemy(LocalBlackboard& _blackboard);
	~ActionApproachEnemy();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

