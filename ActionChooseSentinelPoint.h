#pragma once
#include "Action.h"
class ActionChooseSentinelPoint :
	public Action
{
	// ------------ CSTR & DSTR ------------ //
public:
	ActionChooseSentinelPoint(LocalBlackboard& _blackboard);
	~ActionChooseSentinelPoint();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

