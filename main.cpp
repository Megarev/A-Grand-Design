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
    GUI gui;
public:
    Game() {
        sAppName = "A Grand Design";
    }

    bool OnUserCreate() override {
        
        active_mode = Editor;

        int pixel_size = 20;

        level_size = { ScreenWidth() / pixel_size, ScreenHeight() / pixel_size };
        unit_mgr = UnitManager(level_size, pixel_size);
        unit_renderer = UnitRenderer(level_size, &unit_mgr, this);
        gui = GUI(ScreenWidth(), ScreenHeight());
        gui.GetUnitManager(&unit_mgr);

        gui.SetSpriteSheet(unit_renderer.GetSprite());

        return true;
    }

    bool OnUserUpdate(float dt) override {

        // Input
        olc::vi2d mouse_pos = olc::vi2d(GetMouseX(), GetMouseY());

        if (GetKey(olc::ENTER).bPressed || GetKey(olc::SPACE).bPressed || gui.Run()) {
            if (active_mode == Editor) {
                active_mode = Play;
                unit_mgr.InitializeUnits();
            }
            else if (active_mode == Play) {
                active_mode = Editor;
                unit_mgr.Clear();
            }
        }

        if (active_mode == Editor) {
            unit_mgr.Input(this, mouse_pos);
            if (gui.ClearUnits()) active_mode = Editor;
        }

        // Logic
        gui.Logic(this);
        unit_mgr.Logic(gui.GetSelectedIndex(), dt);
        unit_mgr.GetColor(gui.GetColorFromSlider());

        // Render
        Clear(olc::BLACK);

        unit_mgr.RenderParticles(this);

        if (active_mode == Editor) {
            unit_renderer.PreviewRender();
        }
        else if (active_mode == Play) {
            unit_renderer.PlayRender();
        }
        gui.Render(this);
        
        return true;
    }

    bool OnUserDestroy() override {
        unit_mgr.Clear();
        unit_renderer.Clear();
        gui.Clear();
    
        return true;
    }
};

int main() {

    Game game;
    if (game.Construct(600, 300, 2, 2)) {
        game.Start();
    }

	return 0;
}