#include "InputReinforcementIsValid.h"
#include "GlobalBlackboard.h"

InputReinforcementIsValid::InputReinforcementIsValid(LocalBlackboard& _blackboard)
	: Input(_blackboard) {}

InputReinforcementIsValid::~InputReinforcementIsValid()
{
}

EStatus InputReinforcementIsValid::checkCondition()
{
	if (_blackboard->_reinforcement->isValid()) {
		//GlobalBlackboard::singleton()._state._bug << "This Reinforcement is valid : " << _blackboard->_reinforcement._enemyPos << std::endl;
		//GlobalBlackboard::singleton()._state._bug << "current Ant : " << *_blackboard->p_ant << std::endl;
		return BH_SUCCESS;
	}
	else {
		//GlobalBlackboard::singleton()._state._bug << "This Reinforcement got canceled : " << _blackboard->_reinforcement._enemyPos << std::endl;
		return BH_FAILURE;
	}

}
