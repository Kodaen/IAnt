#pragma once
#include "Action.h"
class ActionApproachSentinelPoint :
    public Action
{
	// ------------ CSTR & DSTR ------------ //
public:
	ActionApproachSentinelPoint(LocalBlackboard& _blackboard);
	~ActionApproachSentinelPoint();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

