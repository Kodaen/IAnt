#pragma once

#include "Behavior.h"

class Action : public Behavior {
	// ------------ CONSTRUCTOR ------------ //
public:
	Action() {}
	~Action() {}


	// ------------ FUNCTIONS ------------ //
protected:
	virtual void onInitialize() { };
	virtual void onTerminate(const EStatus& status) { };

public:
	EStatus update()
	{
		onInitialize();
		EStatus status = doAction();

		onTerminate(status);
		return status;
	}

	virtual EStatus doAction() { return BH_SUCCESS; };
};
