#pragma once

#include "Behavior.h"

class Decorator : public Behavior
{
	// ------------ CSTR & DSTR ------------ //
	public:
		Decorator(LocalBlackboard &_blackboard) : Behavior(_blackboard) {}
		~Decorator() {}


	// ------------ FUNCTIONS ------------ //
protected:
	virtual void onInitialize() { };
	virtual void onTerminate(const EStatus& status) { };
	virtual EStatus update() { return BH_SUCCESS; };
};

