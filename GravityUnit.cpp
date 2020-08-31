#include "GravityUnit.h"

GravityUnit::GravityUnit() {
	color = olc::BLUE;
	is_gravity = true;
}

void GravityUnit::Move(float dt) {
	if (is_connect) pos.x += velocity.x;
	if (is_gravity) pos.y += velocity.y;
}