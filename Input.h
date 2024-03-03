#pragma once

#include "Node.h"

class Input : public Node {
    // ------------ CONSTRUCTOR ------------ //
public:
    Input() {}
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
