#pragma once

#include "Node.h"

class Decorator : public Node
{
	// ------------ CSTR & DSTR ------------ //
	public:
		Decorator() {}
		~Decorator() {}


	// ------------ FUNCTIONS ------------ //
protected:
	virtual void onInitialize() { };
	virtual void onTerminate(const EStatus& status) { };
	virtual EStatus update() { return BH_SUCCESS; };
};

