#include "InputClosestToAnyFood.h"
#include "GlobalBlackboard.h"

InputClosestToFood::InputClosestToFood(LocalBlackboard& _blackboard) : Input(_blackboard)
{
}

InputClosestToFood::~InputClosestToFood()
{
}

EStatus InputClosestToFood::checkCondition()
{
	EStatus res = BH_FAILURE;

	Location* currentAnt = _blackboard->p_ant;
	std::vector<NearbyFoodAnts>& nearbyFoodAnts = GlobalBlackboard::singleton()._nearbyFoodAnts;

	// for each _nearbyFoodAnts pair,check if this ant is nearby any food
	for (NearbyFoodAnts& NFA : nearbyFoodAnts)
	{
		if (NFA.closestAntLoc == *(currentAnt))
		{
			// put every food of which is ant is nearby in the local blackboard
			_blackboard->_nearbyFood.push_back(NFA);
			// return Success
			res = BH_SUCCESS;
		}
	}

	return res;
}
