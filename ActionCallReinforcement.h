#pragma once

#include "Action.h"

class ActionCallReinforcement : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionCallReinforcement(LocalBlackboard& _blackboard);
	~ActionCallReinforcement();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

