#include "ActionExplore.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionExplore::ActionExplore(LocalBlackboard& _blackboard) : Action(_blackboard) {}

ActionExplore::~ActionExplore()
{
}

EStatus ActionExplore::doAction()
{
	EStatus res = BH_FAILURE;
	
	return res;
}
