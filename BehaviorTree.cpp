#include "BehaviorTree.h"
#include "Behavior.h"
#include "Action.h"
#include "Input.h"
#include "InputSuccess.h"
#include "InputFailure.h"
#include "Sequencer.h"
#include "Selector.h"
#include "Decorator.h"
#include "DecoratorAlwaysTrue.h"
#include "DecoratorNot.h"


BehaviorTree::BehaviorTree()
{
	_root = NULL;
	_selectedNode = NULL;
}

void BehaviorTree::execute(const Location& ant)
{
	_root->update();
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
	Behavior* seq = new Sequencer();

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
	Behavior* sel = new Selector();

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
		dec = new DecoratorAlwaysTrue();
		break;

	case DECORATOR_NOT:
		dec = new DecoratorNot();
		break;

	default:
		dec = new Decorator();
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
	default:
		act = new Action();
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
		inp = new InputSuccess();
		break;

	case INPUT_FAILURE:
		inp = new InputFailure();
		break;

	default:
		inp = new Input();
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
