#pragma once

#include "Behavior.h"
#include "Location.h"
#include <string>

enum ENodeType {
	DECORATOR_ALWAYS_TRUE,
	DECORATOR_NOT,

	//DEBUG Nodes
	INPUT_FAILURE,
	INPUT_SUCCESS,
};

class BehaviorTree
{
	// ------------ ATTRIBUTES ------------ //
protected:
	Behavior* _root;


	// ------------ CSTR & DSTR ------------ //
public:
	BehaviorTree();
	// TODO : Implementation vvv
	~BehaviorTree() {}


	// ------------ FUNCTIONS ------------ //
public:
	// TODO : Put ant into the blackboard
	void execute(const Location &ant);

	std::string debugExecute();

	// --------- BUILDER PATTERN --------- //
public :
	BehaviorTree& sequencer();
	BehaviorTree& selector();
	BehaviorTree& decorator(const ENodeType &decoratorType);
	BehaviorTree& action(const ENodeType &actionType);
	BehaviorTree& input(const ENodeType &inputType);

	BehaviorTree& selectParent();

protected:
	Behavior* _selectedNode; // _root by default

	void addChild(Behavior* &node);
};