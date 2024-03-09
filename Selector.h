#pragma once

#include "Behavior.h"

class Selector : public Behavior {
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
        for (Behavior*& child : _children)
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
