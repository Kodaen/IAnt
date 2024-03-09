#pragma once

#include <vector>

#include "LocalBlackboard.h"

enum EStatus
{
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
};

class Behavior 
{
	// ------------ ATTRIBUTES ------------ //
protected :
	std::vector<Behavior*> _children;
	LocalBlackboard* _blackboard;

public : 
	Behavior* _parent;

	// ------------ CSTR & DSTR ------------ //
public:
	Behavior(LocalBlackboard &blackboard) : _parent(NULL), _blackboard(&blackboard) {}
	virtual ~Behavior() {}


	// ------------ FUNCTIONS ------------ //
protected:
	// Called when Behavior starts updating
	virtual void onInitialize() = 0;

	// Called when Behavior ends updating
	virtual void onTerminate(const EStatus& status) = 0;

public:
	virtual EStatus update() = 0;

	void addChild(Behavior* &node) {
		_children.push_back(node);
	}
};

