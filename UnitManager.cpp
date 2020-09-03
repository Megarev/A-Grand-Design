#include "UnitManager.h"

bool UnitManager::IsAUnitConnected(int x, int y, char unit) const {
    if (GetUnit(x, y) == unit) {
        Unit* u = GetUnitAtIndex(x, y);
        if (u != nullptr) return u->is_connect;
    }

    return false;
};

bool UnitManager::IsAUnitWithGravity(int x, int y) const {
    Unit* u = GetUnitAtIndex(x, y);
    if (u != nullptr) return u->is_gravity;

    return false;
}

std::pair<int, bool> UnitManager::CheckUnitNeighbours(int x, int y, char unit) const {
    bool right = (IsAUnitConnected(x + 1, y, unit) && GetConnectIndex(x + 1, y) > 0);
    bool left = (IsAUnitConnected(x - 1, y, unit) && GetConnectIndex(x - 1, y) > 0);
    bool up = (IsAUnitConnected(x, y - 1, unit) && GetConnectIndex(x, y - 1) > 0);
    bool down = (IsAUnitConnected(x, y + 1, unit) && GetConnectIndex(x, y + 1) > 0);
    
    if (right) return std::make_pair(GetConnectIndex(x + 1, y), true);
    if (left) return std::make_pair(GetConnectIndex(x - 1, y), true);
    if (up) return std::make_pair(GetConnectIndex(x, y - 1), true);
    if (down) return std::make_pair(GetConnectIndex(x, y + 1), true);

    return std::make_pair(0, false);
};

UnitManager::UnitManager() {}

UnitManager::UnitManager(const olc::vi2d& size, int psize) 
    : level_size(size), pixel_size(psize) {

    p_system = std::make_unique<CircleParticleSystem>();
	level.resize(level_size.x * level_size.y, '.');
    colors.resize(level_size.x * level_size.y, olc::WHITE);

    current_color = olc::WHITE;

    n_units = 5;
    n_selected_unit = 0;
}

void UnitManager::Input(olc::PixelGameEngine* pge, const olc::vi2d& mouse_pos) {
    olc::vi2d index = mouse_pos / pixel_size;

    if (pge->GetMouse(0).bHeld) {
        if (index.x > 0 && index.y > 0 && index.x < level_size.x - 1 && index.y < level_size.y - 1) {
            switch (n_selected_unit) {
            case Static:    SetUnit(index.x, index.y, '0'); colors[index.y * level_size.x + index.x] = current_color; break;
            case Kinematic: SetUnit(index.x, index.y, '1'); colors[index.y * level_size.x + index.x] = current_color; break;
            case Rotator:   SetUnit(index.x, index.y, '2'); colors[index.y * level_size.x + index.x] = current_color; break;
            case Wall:      SetUnit(index.x, index.y, '3'); colors[index.y * level_size.x + index.x] = current_color; break;
            case Gravity:   SetUnit(index.x, index.y, '4'); colors[index.y * level_size.x + index.x] = current_color; break;
            case Generator: SetUnit(index.x, index.y, '5'); colors[index.y * level_size.x + index.x] = current_color; break;
            }
        }
    }

    // This provides input, useful for player's interaction
    if (pge->GetKey(olc::LEFT).bPressed || pge->GetKey(olc::A).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic || (*it)->id == Gravity) {
                (*it)->velocity = { -(*it)->speed, 0.0f };
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::RIGHT).bPressed || pge->GetKey(olc::D).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic || (*it)->id == Gravity) {
                (*it)->velocity = { (*it)->speed, 0.0f };
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::UP).bPressed || pge->GetKey(olc::W).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic || (*it)->id == Gravity) {
                if ((*it)->is_gravity) {
                    (*it)->velocity.y = -3.0f;
                }
                else {
                    (*it)->velocity = { 0.0f, -(*it)->speed };
                }
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::DOWN).bPressed || pge->GetKey(olc::S).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic || (*it)->id == Gravity) {
                (*it)->velocity = { 0.0f, (*it)->speed };
            }
            it++;
        }
    }

   /* if (pge->GetMouse(2).bPressed || pge->GetKey(olc::R).bPressed) {
        current_color = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);
    }*/

    if (pge->GetMouse(1).bHeld) {
        SetUnit(index.x, index.y, '.');
    }

    if (pge->GetKey(olc::Z).bPressed) {
        level.clear();
        level.resize(level_size.x * level_size.y, '.');
    }

    if (pge->GetKey(olc::Q).bPressed) {
        std::cout << "Unit count : " << units.size() << std::endl;
    }
}

void UnitManager::Logic(int unit, float dt) {
    
    n_selected_unit = unit;
    p_system->Update(dt);
    
    for (auto it = units.begin(); it != units.end();) {
        (*it)->Move(dt);

        if (!(*it)->is_connect && ((*it)->id == Static || (*it)->id == Gravity)) {
            (*it)->is_value = false;
        }

        (*it)->unit_index.x = (*it)->pos.x / pixel_size;
        (*it)->unit_index.y = (*it)->pos.y / pixel_size;

        if ((*it)->is_gravity) {
            (*it)->velocity.y += (*it)->g_speed;
            (*it)->velocity.y = std::fminf((*it)->velocity.y, (*it)->g_max);
        }

        for (auto it2 = units.begin(); it2 != units.end(); it2++) {
            if ((*it2)->is_value && it != it2) {

                if ((*it)->Intersects((*it2)->pos) && !(*it)->is_connect) {
                    if ((*it)->id == Wall) {
                        (*it)->is_value = false;
                        p_system->Input((*it)->pos.x, (*it)->pos.y, olc::Pixel(255, 255, 0, 100));
                    }
                    else if ((*it)->id == Rotator) {
                        (*it2)->velocity.x = -(*it2)->velocity.x;
                        Unit* u = GetUnitAtIndex((int)(*it2)->unit_index.x, (int)(*it2)->unit_index.y + 1);
                        if (u != nullptr) {
                            if (u->id == Kinematic) {
                                u->velocity.x = -u->velocity.x;
                            }
                        }
                    }
                }
            }
        }

        if ((*it)->id == Generator) {
            if ((*it)->is_generate) {
                if (GetUnitAtIndex((*it)->unit_index.x, (*it)->unit_index.y - 1) == nullptr) {
                    Unit* u = GetUnitAtIndex((*it)->unit_index.x, (*it)->unit_index.y + 1);
                    if (u != nullptr && u->id == Static) {
                        Unit* u_cpy = new StaticUnit;
                        u_cpy->Initialize(Static, { u->pos.x, u->pos.y - 2 * pixel_size }, { pixel_size, pixel_size }, { 0.0f, 0.0f }, true, { u->unit_index.x, u->unit_index.y - 2 });

                        (*it)->is_generate = false;
                        units_generated.push_back(u_cpy);
                    }
                }
            }
        }

        if (!(*it)->is_value) {
            it = units.erase(it);
        }
        else {
            it++;
        }
    }

    if (!units_generated.empty()) {
        for (auto it = units_generated.begin(); it != units_generated.end(); it++) {
            units.push_back(*it);
        }
        units_generated.clear();
    }
}

olc::Pixel UnitManager::GetColor(int x, int y) const {
    if (x > 0 && y > 0 && x < level_size.x - 1 && y < level_size.y - 1) {
        return colors[y * level_size.x + x];
    }

    return olc::BLANK;
}

void UnitManager::GetColor(const olc::Pixel& color) {
    current_color = color;
}

void UnitManager::InitializeUnits() {

    for (int y = 1; y < level_size.y - 1; y++) {
        for (int x = 1; x < level_size.x - 1; x++) {
            switch (GetUnit(x, y)) {
            case '0': {
                StaticUnit* s = new StaticUnit;
                s->Initialize(Static, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0.1f, 0 }, false, { x, y });
                s->color = GetColor(x, y);
                units.push_back(s);
                break;
                }
            case '1': {
                KinematicUnit* k = new KinematicUnit;
                k->Initialize(Kinematic, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0.1f, 0 }, true, { x, y });
                k->color = GetColor(x, y);
                units.push_back(k);
                break;
                }
            case '2': {
                RotatorUnit* r = new RotatorUnit;
                r->Initialize(Rotator, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0, 0 }, false, { x, y });
                r->color = GetColor(x, y);
                units.push_back(r);
                break;
                }
            case '3': {
                WallUnit* w = new WallUnit;
                w->Initialize(Wall, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0, 0 }, false, { x, y });
                w->color = GetColor(x, y);
                units.push_back(w);
                break;
                }
            case '4': {
                GravityUnit* g = new GravityUnit;
                g->Initialize(Gravity, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0.1f, 0 }, false, { x, y });
                g->color = GetColor(x, y);
                units.push_back(g);
                }
                break;
            case '5': {
                GeneratorUnit* u = new GeneratorUnit;
                u->Initialize(Generator, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0, 0 }, true, { x, y });
                u->color = GetColor(x, y);
                units.push_back(u);
                }
                break;
            }
        }
    }

    // For the connections to be made, the algorithm needs to check for the connections of already connected units
    // However, it depends on which units get checked first

    for (int y = level_size.y - 1; y > 0; y--) {
        for (int x = level_size.x - 1; x > 0; x--) {
            switch (GetUnit(x, y)) {
            case '0': {
                    Unit* u = GetUnitAtIndex(x, y);

                    if (IsAUnitWithGravity(x + 1, y) || IsAUnitWithGravity(x - 1, y) || IsAUnitWithGravity(x, y - 1) || IsAUnitWithGravity(x, y + 1)) {
                        u->is_gravity = true;
                    }

                    if (GetUnit(x, y + 1) == '1') {
                        u->SetConnectState(true);
                        u->is_connect_index = u->connect_max;
                    }
                    else {
                        if (u != nullptr) {
                            auto neighbour = CheckUnitNeighbours(x, y, '0');

                            if (neighbour.second) {
                                u->SetConnectState(true);
                                u->is_connect_index = neighbour.first - 1;
                            }
                        }
                    }
                }
                break;
            case '1': {
                    Unit* u = GetUnitAtIndex(x, y);
                    if (u != nullptr) {
                        Unit* u2 = GetUnitAtIndex(x, y - 1);
                        if (u2 != nullptr && u2->is_gravity) u->is_gravity = true;
                    }
                }
                break;
            case '4':
                bool is_connect = CheckUnitNeighbours(x, y, '0').second;
                if (is_connect) {
                    Unit* u = GetUnitAtIndex(x, y);
                    if (u != nullptr) u->SetConnectState(true);
                }
                break;
            }
        }
    }

    for (int y = 1; y < level_size.y - 1; y++) {
        for (int x = 1; x < level_size.x - 1; x++) {
            switch (GetUnit(x, y)) {
            case '0': {
                Unit* u = GetUnitAtIndex(x, y);

                if (IsAUnitWithGravity(x + 1, y) || IsAUnitWithGravity(x - 1, y) || IsAUnitWithGravity(x, y - 1) || IsAUnitWithGravity(x, y + 1)) {
                    u->is_gravity = true;
                }

                if (GetUnit(x, y + 1) == '1') {
                    u->SetConnectState(true);
                    u->is_connect_index = u->connect_max;
                }
                else {
                    if (u != nullptr) {
                        auto neighbour = CheckUnitNeighbours(x, y, '0');

                        if (neighbour.second) {
                            u->SetConnectState(true);
                            u->is_connect_index = neighbour.first - 1;
                        }
                    }
                }
            }
                    break;
            case '1': {
                Unit* u = GetUnitAtIndex(x, y);
                if (u != nullptr) {
                    Unit* u2 = GetUnitAtIndex(x, y - 1);
                    if (u2 != nullptr && u2->is_gravity) u->is_gravity = true;
                }
            }
                    break;
            case '4':
                bool is_connect = CheckUnitNeighbours(x, y, '0').second;
                if (is_connect) {
                    Unit* u = GetUnitAtIndex(x, y);
                    if (u != nullptr) u->SetConnectState(true);
                }
                break;
            }
        }
    }
}

// Rendering for particle system
void UnitManager::RenderParticles(olc::PixelGameEngine* pge) {
    p_system->Render(pge);
}

void UnitManager::Clear() {
    for (auto& a : units) {
        delete a;
    }
    units.clear();
}

char UnitManager::GetUnit(int x, int y) const {
    if (x >= 0 && y >= 0 && x < level_size.x && y < level_size.y) {
        return level[y * level_size.x + x];
    }

    return '\0';
}

void UnitManager::SetUnit(int x, int y, char c) {
    if (x >= 0 && y >= 0 && x < level_size.x && y < level_size.y) {
        level[y * level_size.x + x] = c;
    }
}

Unit* UnitManager::GetUnitAtIndex(int x, int y) const {
    for (auto& a : units) {
        if (a->unit_index.x == x && a->unit_index.y == y) {
            return a;
        }
    }

    return nullptr;
}

int UnitManager::GetConnectIndex(int x, int y) const {
    return GetUnitAtIndex(x, y)->is_connect_index;
}

int UnitManager::GetSelectedIndex() const {
    return n_selected_unit;
}

std::vector<Unit*> UnitManager::GetUnits() const { return units; }
std::string UnitManager::GetLevel() const { return level; }

olc::Pixel UnitManager::GetCurrentColor() const { return current_color; }
void UnitManager::SetColor(const olc::Pixel& color) { current_color = color; }

std::vector<olc::Pixel> UnitManager::GetUnitColors() const {
    return colors;
}