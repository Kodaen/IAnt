#pragma once

#include "Action.h"

class ActionFlee : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionFlee(LocalBlackboard& _blackboard);
	~ActionFlee();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

