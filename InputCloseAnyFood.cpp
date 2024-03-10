#include "InputCloseAnyFood.h"

InputCloseAnyFood::InputCloseAnyFood(LocalBlackboard& _blackboard) : Input(_blackboard)
{
}

InputCloseAnyFood::~InputCloseAnyFood()
{
}

EStatus InputCloseAnyFood::checkCondition()
{
	// TODO : IMPL
	// get in local blackboard the Location of my ant
	// for each _nearbyFoodAnts foods of global blackboard, check if 
	// ant is nearby any food EStatus is Success
	// put every food of which is ant is nearby in the local blackboard
	// return Success

	return EStatus();
}
