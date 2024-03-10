#include "BehaviorTree.h"
#include "Behavior.h"
#include "Action.h"
#include "Input.h"
#include "InputSuccess.h"
#include "InputFailure.h"
#include "InputCloseAnyFood.h"
#include "InputClosestToFood.h"
#include "Sequencer.h"
#include "Selector.h"
#include "Decorator.h"
#include "DecoratorAlwaysTrue.h"
#include "DecoratorNot.h"
#include "ActionBlackboardInfos.h"
#include "ActionApproachFood.h"


BehaviorTree::BehaviorTree()
{
	_root = NULL;
	_selectedNode = NULL;
}

void BehaviorTree::execute(Location& ant)
{
	_localBlackboard.p_ant = &ant;

	_root->update();

	// we reset _localBlackboard for next ant.
	_localBlackboard = LocalBlackboard();
}

std::string BehaviorTree::debugExecute()
{
	EStatus res = _root->update();

	switch (res)
	{
	case BH_SUCCESS:
		return "SUCCESS";
		break;

	case BH_FAILURE:
		return "FAILURE";
		break;

	case BH_RUNNING:
		return "RUNNING";
		break;

	default:
		break;
	}
}

// --------- BUILDER PATTERN --------- //

BehaviorTree& BehaviorTree::sequencer()
{
	Behavior* seq = new Sequencer(_localBlackboard);

	if (_root == NULL) {
		_root = seq;
		_selectedNode = _root;
		return *this;
	}

	addChild(seq);
	_selectedNode = seq;
	return *this;
}

BehaviorTree& BehaviorTree::selector()
{
	Behavior* sel = new Selector(_localBlackboard);

	if (_root == NULL) {
		_root = sel;
		_selectedNode = _root;
		return *this;
	}

	addChild(sel);
	_selectedNode = sel;
	return *this;
}

BehaviorTree& BehaviorTree::decorator(const ENodeType& decoratorType)
{
	Behavior* dec;

	switch (decoratorType)
	{
	case DECORATOR_ALWAYS_TRUE:
		dec = new DecoratorAlwaysTrue(_localBlackboard);
		break;

	case DECORATOR_NOT:
		dec = new DecoratorNot(_localBlackboard);
		break;

	default:
		dec = new Decorator(_localBlackboard);
		break;
	}

	if (_root == NULL) {
		_root = dec;
		_selectedNode = _root;
		return *this;
	}

	addChild(dec);
	_selectedNode = dec;
	return *this;
}

BehaviorTree& BehaviorTree::action(const ENodeType& actionType)
{
	Behavior* act;
	switch (actionType)
	{
	case ACTION_BLACKBOARD_INFOS:
		act = new ActionBlackboardInfo(_localBlackboard);
		break;
	case ACTION_APPROACH_FOOD:
		act = new ActionApproachFood(_localBlackboard);
		break;

	default:
		act = new Action(_localBlackboard);
		break;
	}

	if (_root == NULL) {
		_root = act;
		_selectedNode = _root;
		return *this;
	}

	addChild(act);
	return *this;
}

BehaviorTree& BehaviorTree::input(const ENodeType& inputType)
{
	Behavior* inp;
	switch (inputType)
	{
	case INPUT_SUCCESS:
		inp = new InputSuccess(_localBlackboard);
		break;
	case INPUT_FAILURE:
		inp = new InputFailure(_localBlackboard);
		break;
	case INPUT_CLOSE_ANY_FOOD:
		inp = new InputCloseAnyFood(_localBlackboard);
		break;
	case INPUT_CLOSEST_TO_FOOD:
		inp = new InputClosestToFood(_localBlackboard);
		break;
	default:
		inp = new Input(_localBlackboard);
		break;
	}

	if (_selectedNode == NULL) {
		_root = inp;
		_selectedNode = _root;
		return *this;
	}

	addChild(inp);
	return *this;
}

BehaviorTree& BehaviorTree::selectParent()
{
	// TODO : Print error somewhere
	if (_selectedNode == NULL || !_selectedNode->_parent) { return *this; }

	_selectedNode = _selectedNode->_parent;
	return *this;
}

void BehaviorTree::addChild(Behavior*& node)
{
	if (_selectedNode == NULL) { return; }

	node->_parent = _selectedNode;
	_selectedNode->addChild(node);
}
