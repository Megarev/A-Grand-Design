#include "StaticUnit.h"

StaticUnit::StaticUnit() {
	color = olc::GREEN;
}

void StaticUnit::Move(float dt) {
	if (is_connect) pos += velocity;
}