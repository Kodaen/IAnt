#include "ActionCallReinforcement.h"
#include "GlobalBlackboard.h"
#include "Reinforcement.h"

ActionCallReinforcement::ActionCallReinforcement(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionCallReinforcement::~ActionCallReinforcement()
{
}

EStatus ActionCallReinforcement::doAction()
{
	GlobalBlackboard& r_gbb = GlobalBlackboard::singleton();
	Reinforcement r = Reinforcement(*(_blackboard->p_ant), _blackboard->_targetEnemy);

	if (r_gbb.pushReinforcement(r))
		return BH_SUCCESS;
	else
		return BH_FAILURE;
}
