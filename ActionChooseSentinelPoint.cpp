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

	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	auto antLoc = *(_blackboard->p_ant);

	MapSystem::SentinelPoint targetSP;
	if (!_blackboard->_targetSentinelPoint.isNull() //Check if we have a target sentinel point already
		&& _blackboard->_targetSentinelLastVisitedCache == _blackboard->_targetSentinelPoint._lastVisit //Check if the last visit value is still the same, 
		//Another ant explored the point while we were approaching it
		&& r_gbb.isSentinelPointInExplorationCache(_blackboard->_targetSentinelPoint) //Check if the target sentinel point is still in the exploration cache
		&& r_gbb._explorationCacheEntries[_blackboard->_targetSentinelPoint] == antLoc //Check if we are the ant that is supposed to explore the target sentinel point
	)
	{
		res= BH_SUCCESS;
		//The target sentinel point is still valid, we don't need to choose another, we can continue to approach it
		return res;
	}

	//We need to choose a new sentinel point
	//We choose the oldest sentinel point that is not being targeted by another ant
	for each(auto sentinelPoint in MapSystem::getInstance()->getOldestToNewestVisitedSentinelPoint())
	{
		if (!r_gbb.isSentinelPointInExplorationCache(sentinelPoint))
		{
			targetSP = sentinelPoint;
			res = BH_SUCCESS;
			break;
		}
	}

	_blackboard->_targetSentinelPoint = targetSP;
	//We cach the last visit value so if next turn it has changed we know that another ant have visited the sentinel point and we should reevaluate our decision
	_blackboard->_targetSentinelLastVisitedCache = targetSP._lastVisit;

	return res;
}
