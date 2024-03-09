#pragma once

#include "Decorator.h"

class DecoratorNot : public Decorator {
	// ------------ CSTR & DSTR ------------ //
public:
	DecoratorNot();
	~DecoratorNot();

	// ------------ FUNCTIONS ------------ //
public:
	EStatus update();
};