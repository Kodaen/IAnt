#pragma once

#include <vector>

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
	// TODO : Blackboard _blackboard

public : 
	Behavior* _parent;


	// ------------ CSTR & DSTR ------------ //
public:
	Behavior() : _parent(NULL) {}
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

