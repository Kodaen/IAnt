#include "InputCloseAnyFood.h"
#include "GlobalBlackboard.h"

InputCloseAnyFood::InputCloseAnyFood(LocalBlackboard& _blackboard) : Input(_blackboard)
{
}

InputCloseAnyFood::~InputCloseAnyFood()
{
}

EStatus InputCloseAnyFood::checkCondition()
{
	EStatus res = BH_FAILURE;

	Location* currentAnt = _blackboard->p_ant;
	std::vector<NearbyFoodAnts> &nearbyFoodAnts = GlobalBlackboard::singleton()._nearbyFoodAnts;

	// for each _nearbyFoodAnts pair,check if this ant is nearby any food
	for (NearbyFoodAnts& NFA : nearbyFoodAnts)
	{
		if (NFA.nearbyAntsLoc.find(*(currentAnt)) != NFA.nearbyAntsLoc.end())
		{
			// put every food of which is ant is nearby in the local blackboard
			_blackboard->_nearbyFood.push_back(NFA);
			// return Success
			res = BH_SUCCESS;
		}
	}

	return res;
}
