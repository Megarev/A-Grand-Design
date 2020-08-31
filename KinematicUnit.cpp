#include "KinematicUnit.h"

KinematicUnit::KinematicUnit() {
	color = olc::GREY;
}

void KinematicUnit::Move(float dt) {
	pos += velocity;
}