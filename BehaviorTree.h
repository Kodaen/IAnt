#pragma once

#include "Behavior.h"
#include "Location.h"
#include "LocalBlackboard.h"
#include <string>

enum ENodeType {
	DECORATOR_ALWAYS_TRUE,
	DECORATOR_NOT,

	INPUT_CLOSE_ANY_FOOD,
	INPUT_CLOSEST_TO_FOOD,

	ACTION_APPROACH_FOOD,

	//DEBUG Nodes
	INPUT_FAILURE,
	INPUT_SUCCESS,
	ACTION_BLACKBOARD_INFOS
};

class BehaviorTree
{
	// ------------ ATTRIBUTES ------------ //
protected:
	Behavior* _root;
	LocalBlackboard _localBlackboard = LocalBlackboard();

	// ------------ CSTR & DSTR ------------ //
public:
	BehaviorTree();
	// TODO : Implementation vvv
	~BehaviorTree() {}


	// ------------ FUNCTIONS ------------ //
public:
	// TODO : Put ant into the blackboard
	void execute(Location &ant);

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