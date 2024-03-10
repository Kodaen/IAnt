#pragma once

#include "Decorator.h"

class DecoratorNot : public Decorator {
	// ------------ CSTR & DSTR ------------ //
public:
	DecoratorNot(LocalBlackboard& _blackboard);
	~DecoratorNot();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus update();
};