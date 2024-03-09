#include "DecoratorAlwaysTrue.h"

DecoratorAlwaysTrue::DecoratorAlwaysTrue()
{ }

DecoratorAlwaysTrue::~DecoratorAlwaysTrue()
{ }

EStatus DecoratorAlwaysTrue::update()
{
    onInitialize();

    if (_children.size() > 0) {
        EStatus childStatus = _children[0]->update();
        onTerminate(childStatus);
    }

    return BH_SUCCESS;
}
