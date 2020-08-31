#pragma once
#include "olcPixelGameEngine.h"
#include "UnitManager.h"

class UnitRenderer {
private:
    int psize;
    float angle;

    std::vector<olc::vf2d> model;

    // Sprite and Decal
    olc::Sprite* units = nullptr;
    olc::Decal* units_decal = nullptr;

    olc::Pixel colors[5] = { olc::GREEN, olc::WHITE, olc::CYAN, olc::YELLOW, olc::BLUE };

    olc::PixelGameEngine* pge;
    UnitManager* unit_mgr;
    olc::vi2d level_size;
    void Fill_Rect(const olc::vi2d& p1, const olc::vi2d& p2, const olc::vi2d& p3, const olc::vi2d& p4, const olc::Pixel& color, olc::Pixel outlineColor = olc::WHITE);

    void FillRotateRect(const std::vector<olc::vf2d>& model, float angle, const olc::vf2d& position, const olc::vf2d& origin, const olc::Pixel& color, olc::Pixel outlineColor = olc::WHITE);
public:
    UnitRenderer();
    UnitRenderer(const olc::vi2d& size, UnitManager* mgr, olc::PixelGameEngine* p);

    void PreviewRender();
    void PlayRender();

    void Clear();
};