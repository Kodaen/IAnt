#pragma once

#include "Action.h"

class ActionCharge : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionCharge(LocalBlackboard& _blackboard);
	~ActionCharge();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

