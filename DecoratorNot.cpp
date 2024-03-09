#include "DecoratorNot.h"

DecoratorNot::DecoratorNot()
{
}

DecoratorNot::~DecoratorNot()
{
}

EStatus DecoratorNot::update()
{
	onInitialize();

	if (_children.size() == 0) { return BH_FAILURE; }

	EStatus childStatus = _children[0]->update();

	if (childStatus == BH_FAILURE)
		childStatus = BH_SUCCESS;
	else if (childStatus == BH_SUCCESS)
		childStatus = BH_FAILURE;

	onTerminate(childStatus);
	return childStatus;
}
