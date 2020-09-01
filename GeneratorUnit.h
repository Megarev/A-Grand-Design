#pragma once
#include "Unit.h"

class GeneratorUnit : public Unit {
private:
	float accumulator, reset_generate;
public:
	GeneratorUnit();

	void Move(float dt) override;
};