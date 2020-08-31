#pragma once
#include "olcPixelGameEngine.h"

class Unit {
public:
    olc::vf2d pos, velocity;
    olc::vi2d size, unit_index;
    float speed, g_speed;
    bool is_connect; // Is the given unit connected to other unit
    olc::Pixel color;
    int id, is_connect_index, connect_max;
    bool is_value, is_gravity;

    Unit();

    virtual void Move(float) {}
    void SetConnectState(bool state);

    bool Intersects(const olc::vi2d& unit_pos);

    void Initialize(int unit_id, const olc::vf2d& p, const olc::vi2d& s, const olc::vf2d& v, bool is_connected = false, olc::vi2d index = olc::vi2d(0, 0));

    void Draw(olc::PixelGameEngine* pge);
};
