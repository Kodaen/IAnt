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

	auto antLoc = *(_blackboard->p_ant);
	
	/*if (r_gbb.singleton().doMoveLocation(antLoc, moveLocation)) {
		res = BH_SUCCESS;
	}*/
	
	return res;
}
