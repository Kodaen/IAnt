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

	if (r_gbb.isAntInExplorationCache(antLoc))
	{
		_blackboard->_targetSentinelPoint = r_gbb._explorationCacheAntToSP[antLoc];
		r_gbb._state._bug << "Ant " << antLoc << " is still moving towards sentinel point " << _blackboard->_targetSentinelPoint._location << std::endl;
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

	r_gbb._state._bug << "Ant " << antLoc << " has chosen a new sentinel point " << targetSP._location;
	_blackboard->_targetSentinelPoint = targetSP;
	r_gbb._state._bug << " " << _blackboard->_targetSentinelPoint._location << std::endl;

	return res;
}
