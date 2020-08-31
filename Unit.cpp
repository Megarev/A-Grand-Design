#include "Unit.h"

Unit::Unit() {
	is_connect = false;
    is_gravity = false;
    is_value = true;

    speed = 0.1f;
    g_speed = 0.2f;
    g_max = 0.4f;

    is_connect_index = 0;
    connect_max = 5;
}

void Unit::Initialize(int unit_id, const olc::vf2d& p, const olc::vi2d& s, const olc::vf2d& v, bool is_connected, olc::vi2d index) {
    pos = p;
    size = s;
    velocity = v;
    is_connect = is_connected;
    id = unit_id;
    unit_index = index;
}

void Unit::SetConnectState(bool state) {
    is_connect = state;
}

bool Unit::Intersects(const olc::vi2d& unit_pos) {
    return (pos.x < unit_pos.x + size.x && pos.x + size.x > unit_pos.x &&
            pos.y < unit_pos.y + size.y && pos.y + size.y > unit_pos.y);
}

void Unit::Draw(olc::PixelGameEngine* pge) {
    pge->FillRect((olc::vi2d)pos, size, color);
}