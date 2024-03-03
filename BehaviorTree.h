#pragma once

#include "Node.h"
#include "Location.h"
#include <string>

class BehaviorTree
{
	// ------------ ATTRIBUTES ------------ //
protected:
	Node* _root;


	// ------------ CSTR & DSTR ------------ //
public:
	BehaviorTree(Node* root) : _root(root) {};
	BehaviorTree() : _root() {};
	~BehaviorTree() {}


	// ------------ FUNCTIONS ------------ //
public:
	void execute(const Location& ant);

	std::string debugExecute();
};

