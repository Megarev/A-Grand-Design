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

    olc::Sprite* logo = nullptr;
    olc::Decal* decal_logo = nullptr;

    bool is_in_splash_screen;
    float accumulator, delay;
    int alpha;
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

        is_in_splash_screen = true;

        logo = new olc::Sprite("files/images/logo.png");
        decal_logo = new olc::Decal(logo);
        alpha = 300;

        accumulator = 0.0f;
        delay = 3.0f;

        return true;
    }

    bool OnUserUpdate(float dt) override {

        // Splash screen
        if (accumulator < delay) {
            accumulator += dt;

            uint8_t alpha_state = alpha-- > 255 ? 255 : alpha;
            if (alpha_state < 0) alpha_state = 0;

            Clear(olc::BLACK);
            DrawDecal({ 0, 0 }, decal_logo, { 1.0f, 1.0f }, olc::Pixel(255, 255, 255, alpha_state));
            DrawStringDecal({ ScreenWidth() - 320.0f, ScreenHeight() - 10.0f }, "Made for the olcGameJam 2020 by Megarev", olc::Pixel(0, 100, 255, alpha_state));
        }
        else {
            // Inside Editor

            // Input
            olc::vi2d mouse_pos = olc::vi2d(GetMouseX(), GetMouseY());

            if (gui.Run()) {
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
        }

        return true;
    }

    bool OnUserDestroy() override {
        unit_mgr.Clear();
        unit_renderer.Clear();
        gui.Clear();

        delete decal_logo;
        delete logo;
    
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