#include "ActionApproachSentinelPoint.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionApproachSentinelPoint::ActionApproachSentinelPoint(LocalBlackboard& _blackboard) : Action(_blackboard) {}

ActionApproachSentinelPoint::~ActionApproachSentinelPoint()
{
}

EStatus ActionApproachSentinelPoint::doAction()
{
	EStatus res = BH_FAILURE;

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	auto antLoc = *(_blackboard->p_ant);
	auto moveLocation=MapSystem::getInstance()->moveToward(antLoc, _blackboard->_targetSentinelPoint._location);
	auto endTurnLocation = antLoc;

	if (r_gbb.singleton().doMoveLocation(antLoc, moveLocation)) {
		endTurnLocation = moveLocation;
		res = BH_SUCCESS;
	}

	r_gbb._explorationCacheEntries[_blackboard->_targetSentinelPoint] = endTurnLocation;
	
	return res;
}
