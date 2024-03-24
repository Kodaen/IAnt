#pragma once
#include "Action.h"
class ActionExplore :
    public Action
{
	// ------------ CSTR & DSTR ------------ //
public:
	ActionExplore(LocalBlackboard& _blackboard);
	~ActionExplore();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus doAction();
};

