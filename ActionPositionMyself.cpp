#include "ActionPositionMyself.h"

ActionPositionMyself::ActionPositionMyself(LocalBlackboard& _blackboard)
	: Action(_blackboard) {}

ActionPositionMyself::~ActionPositionMyself()
{
}

EStatus ActionPositionMyself::doAction()
{
	return EStatus();
}
