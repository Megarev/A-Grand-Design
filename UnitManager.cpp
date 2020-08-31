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

    n_selected_unit = 0;
}

void UnitManager::Input(olc::PixelGameEngine* pge, const olc::vi2d& mouse_pos) {
    olc::vi2d index = mouse_pos / pixel_size;

    if (pge->GetMouse(0).bHeld) {
        switch (n_selected_unit) {
        case Static: SetUnit(index.x, index.y, '0'); break;
        case Kinematic: SetUnit(index.x, index.y, '1'); break;
        case Rotator: SetUnit(index.x, index.y, '2'); break;
        case Wall: SetUnit(index.x, index.y, '3'); break;
        case Gravity: SetUnit(index.x, index.y, '4'); break;
        }
    }

    if (pge->GetKey(olc::LEFT).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic) {
                (*it)->velocity = { -(*it)->speed, 0.0f };
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::RIGHT).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic) {
                (*it)->velocity = { (*it)->speed, 0.0f };
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::UP).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic) {
                (*it)->velocity = { 0.0f, -(*it)->speed };
            }
            it++;
        }
    }
    else if (pge->GetKey(olc::DOWN).bPressed) {
        for (auto it = units.begin(); it != units.end();) {
            if ((*it)->id == Static || (*it)->id == Kinematic) {
                (*it)->velocity = { 0.0f, (*it)->speed };
            }
            it++;
        }
    }

    if (pge->GetMouseWheel() > 0) n_selected_unit++;
    if (pge->GetMouseWheel() < 0) n_selected_unit--;
    if (n_selected_unit > 4) n_selected_unit = 4;
    if (n_selected_unit < 0) n_selected_unit = 0;

    if (pge->GetMouse(1).bHeld) {
        SetUnit(index.x, index.y, '.');
    }

    if (pge->GetKey(olc::A).bPressed) {
        level.clear();
        level.resize(level_size.x * level_size.y, '.');
    }
}

void UnitManager::Logic(float dt) {
    
    p_system->Update(dt);

    for (auto it = units.begin(); it != units.end();) {
        (*it)->Move(dt);

        if (!(*it)->is_connect && (*it)->id == Static) {
            (*it)->is_value = false;
        }

        (*it)->unit_index.x = (*it)->pos.x / pixel_size;
        (*it)->unit_index.y = (*it)->pos.y / pixel_size;

        if ((*it)->is_gravity) {
            (*it)->pos.y += (*it)->g_speed;
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
                                u->velocity.y = -u->velocity.y;
                            }
                        }
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
}

void UnitManager::InitializeUnits() {

    for (int y = 1; y < level_size.y - 1; y++) {
        for (int x = 1; x < level_size.x - 1; x++) {
            switch (GetUnit(x, y)) {
            case '0': {
                StaticUnit* s = new StaticUnit;
                s->Initialize(Static, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0.1f, 0 }, false, { x, y });
                units.push_back(s);
                break;
                }
            case '1': {
                KinematicUnit* k = new KinematicUnit;
                k->Initialize(Kinematic, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0.1f, 0 }, true, { x, y });
                units.push_back(k);
                break;
                }
            case '2': {
                RotatorUnit* r = new RotatorUnit;
                r->Initialize(Rotator, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0, 0 }, false, { x, y });
                units.push_back(r);
                break;
                }
            case '3': {
                WallUnit* w = new WallUnit;
                w->Initialize(Wall, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 10, 0 }, false, { x, y });
                units.push_back(w);
                break;
                }
            case '4': {
                GravityUnit* g = new GravityUnit;
                g->Initialize(Gravity, { x * (float)pixel_size, y * (float)pixel_size }, { pixel_size, pixel_size }, { 0, 0 }, false, { x, y });
                units.push_back(g);
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
                        if (GetUnitAtIndex(x, y - 1)->is_gravity) u->is_gravity = true;
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
                    if (GetUnitAtIndex(x, y - 1)->is_gravity) u->is_gravity = true;
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