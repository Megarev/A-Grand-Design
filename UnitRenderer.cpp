#include "UnitRenderer.h"

UnitRenderer::UnitRenderer() {}

UnitRenderer::UnitRenderer(const olc::vi2d& size, UnitManager* mgr, olc::PixelGameEngine* p)
    : pge(p), unit_mgr(mgr), level_size(size) {
    psize = 20;
    angle = 0.0f;
    n_units = 6;

    units = new olc::Sprite("files/images/Tileset.png");
    units_decal = new olc::Decal(units);

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

            olc::Pixel color = unit_mgr->GetColor(j, i);

            switch (unit_mgr->GetUnit(j, i)) {
            case '0':
                //pge->FillRect(j * psize, i * psize, psize, psize, olc::GREEN);
                //pge->DrawPartialSprite(j * psize, i * psize, units, 0, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { 0.0f, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            case '1':
                //pge->FillRect(j * psize, i * psize, psize, psize, olc::GREY);
                //pge->DrawPartialSprite(j * psize, i * psize, units, psize, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            case '2':
                //pge->FillRect(j * psize, i * psize, psize, psize, olc::CYAN);
                //pge->DrawPartialSprite(j * psize, i * psize, units, 2 * psize, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { 2 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            case '3':
                //pge->FillRect(j * psize, i * psize, psize, psize, olc::YELLOW);
                //pge->DrawPartialSprite(j * psize, i * psize, units, 3 * psize, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { 3 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            case '4':
                //pge->FillRect(j * psize, i * psize, psize, psize, olc::BLUE);
                //pge->DrawPartialSprite(j * psize, i * psize, units, 4 * psize, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { 4 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            case '5':
                //pge->DrawPartialSprite(j * psize, i * psize, units, 5 * psize, 0, psize, psize);
                pge->DrawPartialDecal({ j * (float)psize, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { 5 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
                break;
            }
        }
    }

    //pge->FillRect(0, 0, psize, psize, colors[unit_mgr->GetSelectedIndex()]);
    //pge->DrawPartialSprite(0, 0, units, unit_mgr->GetSelectedIndex() * psize, 0, psize, psize);
    for (int i = 0; i < n_units; i++) {
        //pge->DrawPartialSprite(1, i * psize + 1, units, i * psize, 0, psize, psize);
        pge->DrawPartialDecal({ 1.0f, i * (float)psize }, { (float)psize, (float)psize }, units_decal, { i * (float)psize, 0.0f }, { (float)psize, (float)psize });
    }
}

void UnitRenderer::PlayRender() {
    angle += 0.01f;

    for (const auto& a : unit_mgr->GetUnits()) {

        olc::Pixel color = a->color;
        auto [x, y] = a->pos;

        switch (a->id) {
        case 0:
            //pge->DrawPartialSprite(a->pos.x, a->pos.y, units, 0, 0, psize, psize);
            pge->DrawPartialDecal({ x, y }, { (float)psize, (float)psize }, units_decal, { 0.0f, 0.0f }, { (float)psize, (float)psize }, color);
            break;
        case 1:
            //FillRotateRect(model, angle, a->pos + olc::vf2d(5.0f, 0.0f), { -psize / 2.0f, -psize / 2.0f }, a->color);
            pge->DrawPartialRotatedDecal(olc::vf2d(x + psize / 2.0f, y), units_decal, angle, { psize / 2.0f, psize / 2.0f }, { (float)psize, 0.0f }, { (float)psize, (float)psize }, { 1.0f, 1.0f }, color);
            break;
        case 2:
            //pge->DrawPartialSprite(a->pos.x, a->pos.y, units, 2 * psize, 0, psize, psize);
            pge->DrawPartialDecal({ x, y }, { (float)psize, (float)psize }, units_decal, { 2 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
            break;
        case 3:
            pge->DrawPartialDecal({ x, y }, { (float)psize, (float)psize }, units_decal, { 3 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
            break;
        case 4:
            //pge->FillRect(a->pos, a->size, a->color);
            //pge->DrawPartialSprite(a->pos.x, a->pos.y, units, 4 * psize, 0, psize, psize);
            pge->DrawPartialDecal({ x, y }, { (float)psize, (float)psize }, units_decal, { 4 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
            break;
        case 5:
            //pge->DrawPartialSprite(a->pos.x, a->pos.y, units, 5 * psize, 0, psize, psize);
            pge->DrawPartialDecal({ x, y }, { (float)psize, (float)psize }, units_decal, { 5 * (float)psize, 0.0f }, { (float)psize, (float)psize }, color);
            break;
        }
    }
}

olc::Sprite* UnitRenderer::GetSprite() {
    return units;
}

void UnitRenderer::Clear() {
    delete units_decal;
    delete units;
}