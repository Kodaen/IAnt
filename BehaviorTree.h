#pragma once

#include "Behavior.h"
#include "Location.h"
#include "LocalBlackboard.h"
#include <string>

enum ENodeType {
	DECORATOR_ALWAYS_TRUE,
	DECORATOR_NOT,

	INPUT_CLOSEST_TO_FOOD,
	INPUT_I_DIE_BY_GOING_THERE,
	INPUT_ENEMY_NEAR,
	INPUT_WE_BOTH_DIE,
	INPUT_CLOSEST_TO_MY_HILL,
	INPUT_CALLED_FOR_REINFORCEMENT,
	INPUT_REINFORCEMENT_IS_VALID,
	INPUT_ALL_ANTS_IN_POSITION,

	ACTION_APPROACH_FOOD,
	ACTION_CALC_TRAJ_FOR_FOOD,
	ACTION_GET_CLOSEST_FOOD,
	ACTION_APPROACH_ENEMY,
	ACTION_CALL_REINFORCEMENT,
	ACTION_CHARGE,
	ACTION_FLEE,

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
	void execute(Location& ant);

	std::string debugExecute();

	// --------- BUILDER PATTERN --------- //
public:
	BehaviorTree& sequencer();
	BehaviorTree& selector();
	BehaviorTree& decorator(const ENodeType& decoratorType);
	BehaviorTree& action(const ENodeType& actionType);
	BehaviorTree& input(const ENodeType& inputType);

	BehaviorTree& selectParent();

protected:
	Behavior* _selectedNode; // _root by default

	void addChild(Behavior*& node);
};