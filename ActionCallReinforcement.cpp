#include "ActionCallReinforcement.h"
#include "Reinforcement.h"

ActionCallReinforcement::ActionCallReinforcement(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionCallReinforcement::~ActionCallReinforcement()
{
}

EStatus ActionCallReinforcement::doAction()
{
	Reinforcement r = Reinforcement(*(_blackboard->p_ant), _blackboard->_targetEnemy);

	return BH_SUCCESS;
}
