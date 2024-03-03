#pragma once

#include "Node.h"

class Sequencer : public Node {
    // ------------ CONSTRUCTOR ------------ //
public:
    Sequencer() {}


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

            if (childStatus == BH_FAILURE || childStatus == BH_RUNNING)
            {
                onTerminate(childStatus);
                return childStatus;
            }
        }
        onTerminate(BH_SUCCESS);
        return BH_SUCCESS;
    }
};