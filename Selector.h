#pragma once

#include "Node.h"

class Selector : public Node {
    // ------------ CONSTRUCTOR ------------ //
public:
    Selector() {}
    ~Selector() {}


    // ------------ FUNCTIONS ------------ //
protected:
    virtual void onInitialize() { };
    virtual void onTerminate(const EStatus& status) { };

public:
    EStatus update()
    {
        onInitialize();
        for (Node*& child : _children)
        {
            EStatus childStatus = child->update();

            if (childStatus == BH_SUCCESS || childStatus == BH_RUNNING)
            {
                onTerminate(childStatus);
                return childStatus;
            }
        }
        onTerminate(BH_FAILURE);
        return BH_FAILURE;
    }
};
