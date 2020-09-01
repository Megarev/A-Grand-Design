#include "GeneratorUnit.h"

GeneratorUnit::GeneratorUnit() {
	color = olc::RED;
	accumulator = 0.0f;
	reset_generate = 5.0f;
	is_generate = true;
}

void GeneratorUnit::Move(float dt) {
	if (!is_generate) {
		accumulator += dt;
		if (accumulator > reset_generate) {
			is_generate = true;
		}
	}
	else {
		accumulator = 0.0f;
	}
	
}