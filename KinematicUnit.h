#pragma once
#include "Unit.h"
class KinematicUnit : public Unit {
public:
    KinematicUnit();

    void Move(float dt) override;
};