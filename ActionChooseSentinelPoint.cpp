#include "ActionChooseSentinelPoint.h"
#include "GlobalBlackboard.h"
#include "MapSystem.h"

ActionChooseSentinelPoint::ActionChooseSentinelPoint(LocalBlackboard& _blackboard) : Action(_blackboard) {}

ActionChooseSentinelPoint::~ActionChooseSentinelPoint()
{
}

EStatus ActionChooseSentinelPoint::doAction()
{
	EStatus res = BH_FAILURE;

	auto antLoc = *(_blackboard->p_ant);

	MapSystem::SentinelPoint targetSP;
	if (false) //If we're in the cache
	{
		targetSP = Location(0, 0);
		return res;
	}

	targetSP = MapSystem::getInstance()->getOldestVisitedSentinelPoint();
	_blackboard->_targetSentinelPoint = targetSP._location;
	//We cach the last visit value so if next turn it has changed we know that another ant have visited the sentinel point and we should reevaluate our decision
	_blackboard->_targetSentinelLastVisitedCache = targetSP._lastVisit;

	return res;
}
