#pragma once

#include "Decorator.h"

class DecoratorAlwaysTrue : public Decorator {
	// ------------ CSTR & DSTR ------------ //
public:
	DecoratorAlwaysTrue();
	~DecoratorAlwaysTrue();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus update();
};