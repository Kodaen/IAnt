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

	auto antLoc = *(_blackboard->p_ant);

	//First choice: Get the closest unknown anthill and explore it
	auto targetLocation=MapSystem::getInstance()->getClosestUnknownAnthill(antLoc);
	//Second choice: No more ant hills ? Just don't move, who fucking cares
	if (targetLocation == NULL_LOCATION) 
	{
		targetLocation = antLoc;
	}

	auto moveLocation = MapSystem::getInstance()->moveToward(antLoc, targetLocation);

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	GlobalBlackboard::singleton()._state._bug << "Exploring, going to :" << targetLocation << " from: "<<antLoc<<" passing to: "<<moveLocation<<std::endl;
	if (r_gbb.singleton().doMoveLocation(antLoc, moveLocation)) {
		res = BH_SUCCESS;
	}
	
	return res;
}
