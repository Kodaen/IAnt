#pragma once

#include <vector>

enum EStatus
{
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
};

class Node 
{
	// ------------ ATTRIBUTES ------------ //
protected :
	std::vector<Node*> _children;
	// TODO : Blackboard _blackboard

public : 
	Node* _parent;


	// ------------ CSTR & DSTR ------------ //
public:
	Node() : _parent(NULL) {}
	virtual ~Node() {}


	// ------------ FUNCTIONS ------------ //
protected:
	// Called when node starts updating
	virtual void onInitialize() = 0;

	// Called when node ends updating
	virtual void onTerminate(const EStatus& status) = 0;

public:
	virtual EStatus update() = 0;

	void addChild(Node* &node) {
		_children.push_back(node);
	}
};

