#pragma once
#include "Unit.h"

class GravityUnit : public Unit {
public:
	GravityUnit();

	void Move(float dt) override;
};

