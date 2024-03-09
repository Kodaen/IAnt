#pragma once

#include "Behavior.h"

class Input : public Behavior {
    // ------------ CSTR & DSTR ------------ //
public:
    Input(LocalBlackboard &_blackboard) : Behavior(_blackboard) {}
    ~Input() {}


    // ------------ FUNCTIONS ------------ //
protected:
    virtual void onInitialize() { };
    virtual void onTerminate(const EStatus& status) { };

public:
    EStatus update()
    {
        onInitialize();
        EStatus status = checkCondition();

        onTerminate(status);
        return status;
    }

    virtual EStatus checkCondition() { return BH_SUCCESS; };
};
