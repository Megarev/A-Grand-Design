#include "libs.h"

class Game : public olc::PixelGameEngine {
private:
    enum Mode {
        Editor,
        Play
    } active_mode;
    
    olc::vi2d level_size;
    UnitManager unit_mgr;
    UnitRenderer unit_renderer;

    olc::Pixel colors[4] = { olc::GREEN, olc::WHITE, olc::CYAN, olc::YELLOW };

    std::vector<olc::vf2d> model;
public:
    Game() {
        sAppName = "A Grand Design";
    }

    bool OnUserCreate() override {
        
        active_mode = Editor;

        level_size = { 20, 20 };
        unit_mgr = UnitManager(level_size, 10);
        unit_renderer = UnitRenderer(level_size, &unit_mgr, this);

        return true;
    }

    bool OnUserUpdate(float dt) override {

        // Input
        olc::vi2d mouse_pos = olc::vi2d(GetMouseX(), GetMouseY());

        if (GetKey(olc::ENTER).bPressed || GetKey(olc::SPACE).bPressed) {
            if (active_mode == Editor) {
                active_mode = Play;
                unit_mgr.InitializeUnits();
            }
            else if (active_mode == Play) {
                active_mode = Editor;
                unit_mgr.Clear();
            }
        }

        unit_mgr.Input(this, mouse_pos);

        // Logic
        unit_mgr.Logic(dt);

        // Render
        Clear(olc::BLACK);
        unit_mgr.RenderParticles(this);

        if (active_mode == Editor) {
            unit_renderer.PreviewRender();
        }
        else if (active_mode == Play) {
            unit_renderer.PlayRender();
        }
        return true;
    }

    bool OnUserDestroy() override {
        unit_mgr.Clear();
    
        return true;
    }
};

int main() {

    Game game;
    if (game.Construct(200, 200, 2, 2)) {
        game.Start();
    }

	return 0;
}