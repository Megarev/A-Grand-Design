#pragma once
#include "Unit.h"

// Static objects
class StaticUnit : public Unit {
public:
    StaticUnit();

    void Move(float dt) override;
};