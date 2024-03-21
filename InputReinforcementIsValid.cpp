#include "InputReinforcementIsValid.h"

InputReinforcementIsValid::InputReinforcementIsValid(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputReinforcementIsValid::~InputReinforcementIsValid()
{
}

EStatus InputReinforcementIsValid::checkCondition()
{
	if (_blackboard->_reinforcement.isValid())
		return BH_SUCCESS;
	else
		return BH_FAILURE;

}
