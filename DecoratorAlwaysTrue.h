#pragma once

#include "Decorator.h"

class DecoratorAlwaysTrue : public Decorator {
	// ------------ CSTR & DSTR ------------ //
public:
	DecoratorAlwaysTrue(LocalBlackboard &_blakboard);
	~DecoratorAlwaysTrue();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus update();
};