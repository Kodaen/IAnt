#pragma once

#include "Action.h"

class ActionPositionMyself : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionPositionMyself(LocalBlackboard& _blackboard);
	~ActionPositionMyself();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

