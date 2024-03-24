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
	auto targetLocation=MapSystem::getInstance()->getOldestVisitedSentinelPoint();
	auto moveLocation = MapSystem::getInstance()->moveToward(antLoc, targetLocation);
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	GlobalBlackboard::singleton()._state._bug << "Exploring, going to :" << targetLocation << " from: "<<antLoc<<" passing to: "<<moveLocation<<std::endl;
	if (r_gbb.singleton().doMoveLocation(antLoc, moveLocation)) {
		res = BH_SUCCESS;
	}
	
	return res;
}
