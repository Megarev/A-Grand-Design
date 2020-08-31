#include "UnitRenderer.h"

UnitRenderer::UnitRenderer() {}

UnitRenderer::UnitRenderer(const olc::vi2d& size, UnitManager* mgr, olc::PixelGameEngine* p)
    : pge(p), unit_mgr(mgr), level_size(size) {
    psize = 10;
    angle = 0.0f;

    model = {
        { 0.0f, 0.0f },
        { (float)psize, 0.0f },
        { (float)psize, (float)psize },
        { 0.0f, (float)psize }
    };
}

void UnitRenderer::Fill_Rect(const olc::vi2d& p1, const olc::vi2d& p2, const olc::vi2d& p3, const olc::vi2d& p4, const olc::Pixel& color, olc::Pixel outlineColor) {
    pge->FillTriangle(p1, p2, p3, color);
    pge->FillTriangle(p1, p4, p3, color);

    // Outline
    pge->DrawLine(p1, p2, outlineColor);
    pge->DrawLine(p2, p3, outlineColor);
    pge->DrawLine(p3, p4, outlineColor);
    pge->DrawLine(p4, p1, outlineColor);
}

void UnitRenderer::FillRotateRect(const std::vector<olc::vf2d>& model, float angle, const olc::vf2d& position, const olc::vf2d& origin, const olc::Pixel& color, olc::Pixel outlineColor) {
    int nVertices = 4; // Square
    std::vector<olc::vf2d> newCoords(nVertices);

    // Rotation
    for (int i = 0; i < nVertices; i++) {
        newCoords[i].x = position.x + (model[i].x + origin.x) * cosf(angle) - (model[i].y + origin.y) * sinf(angle);
        newCoords[i].y = position.y + (model[i].x + origin.x) * sinf(angle) + (model[i].y + origin.y) * cosf(angle);
    }

    Fill_Rect(newCoords[0], newCoords[1], newCoords[2], newCoords[3], color, outlineColor);
}

void UnitRenderer::PreviewRender() {

    for (int i = 0; i < level_size.y; i++) {
        for (int j = 0; j < level_size.x; j++) {
            switch (unit_mgr->GetUnit(j, i)) {
            case '0':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::GREEN);
                break;
            case '1':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::GREY);
                break;
            case '2':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::CYAN);
                break;
            case '3':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::YELLOW);
                break;
            case '4':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::BLUE);
                break;
            case '.':
                pge->FillRect(j * psize, i * psize, psize, psize, olc::BLACK);
                break;
            }
        }
    }

    pge->FillRect(0, 0, psize, psize, colors[unit_mgr->GetSelectedIndex()]);
}

void UnitRenderer::PlayRender() {
    angle += 0.01f;

    for (auto& a : unit_mgr->GetUnits()) {
        switch (a->id) {
        case 0:
        case 2:
        case 3:
        case 4:
            pge->FillRect(a->pos, a->size, a->color);
            break;
        case 1:
            FillRotateRect(model, angle, a->pos + olc::vf2d(5.0f, 0.0f), { -psize / 2.0f, -psize / 2.0f }, a->color);
            break;
        }
    }
}