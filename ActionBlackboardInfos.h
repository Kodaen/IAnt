#pragma once

#include "Action.h"
#include "GlobalBlackboard.h"

class ActionBlackboardInfo : public Action {
	// ------------ CSTR & DSTR ------------ //
public:
	ActionBlackboardInfo(LocalBlackboard& _blackboard) : Action(_blackboard) {}
	~ActionBlackboardInfo() {}

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction() { 
		GlobalBlackboard& globalBlackboard = GlobalBlackboard::singleton();
		
		globalBlackboard._state._bug << "Current ant : (" << (_blackboard->p_ant)->_row << "," << (_blackboard->p_ant)->_col << ")" << std::endl;
		return BH_SUCCESS;
	};
};