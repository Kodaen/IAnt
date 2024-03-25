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

	res=BH_SUCCESS;
	r_gbb._explorationCacheSPToAnt[_blackboard->_targetSentinelPoint] = endTurnLocation;
	r_gbb._explorationCacheAntToSP[endTurnLocation] = _blackboard->_targetSentinelPoint;
	r_gbb._state._bug<<"Ant "<<antLoc<<" is moving towards sentinel point "<<_blackboard->_targetSentinelPoint._location<<std::endl;

	return res;
}
