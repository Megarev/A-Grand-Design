#include "UI.h"

GUI::GUI() {}

GUI::GUI(int screen_width, int screen_height) {
	psize = 20;
	n_units = 6;

	selected_button = olc::WHITE;
	selected_button_pos = { 0, psize };

	color_slider = Slider({ screen_width - psize + 1, psize + 2 }, 1);
	run_button = ToggleButton("files/images/UIButtons.png", 0, { screen_width - psize, 1 }, { psize, psize });

	border_color = olc::Pixel(100, 10, 255);

	for (int i = 0; i < n_units; i++) {
		buttons.push_back(SpriteButton(i, { 0, (i + 1) * psize }, { psize, psize }));
	}
	clear_button = SpriteButton((int)buttons.size() + 1, { 1, screen_height - psize }, { psize, psize });
}

void GUI::GetUnitManager(UnitManager* u) {
	unit_mgr = u;
}

void GUI::SetSpriteSheet(olc::Sprite* spritesheet) {
	for (auto& b : buttons) {
		b.GetSpriteSheet(spritesheet);
	}
	clear_button.GetSpriteSheet(spritesheet);
}

bool GUI::ClearUnits() {
	if (clear_button.GetIsPressed()) {
		unit_mgr->ClearLevel();
		return true;
	}

	return false;
}

bool GUI::Run() {
	return run_button.GetIsPressed();
}

void GUI::Logic(olc::PixelGameEngine* pge) {
	for (auto& b : buttons) {
		b.Logic(pge);

		if (b.GetIsPressed()) {
			selected_index = b.GetID();
		}
	}

	color_slider.Input(pge);
	color_slider.Logic();

	run_button.Logic(pge);
	clear_button.Logic(pge);
}

void GUI::Render(olc::PixelGameEngine* pge) {

	// Border around the window

	pge->DrawRect(psize + 1, psize + 1, pge->ScreenWidth() - 2 * (psize + 1), pge->ScreenHeight() - 2 * (psize + 1), border_color);
	pge->FillRectDecal({ 1, (float)psize * (selected_index + 1) }, { (float)psize, (float)psize }, olc::Pixel(255, 255, 255, 100));

	for (int i = 0; i < n_units; i++) {
		buttons[i].Render(i, pge);
	}

	color_slider.Render(pge);
	run_button.Render(0, pge);

	clear_button.Render(0, pge, 1);
}

int GUI::GetSelectedIndex() const {
	return selected_index;
}

olc::Pixel GUI::GetColorFromSlider() const {
	return color_slider.GetColor();
}

void GUI::Clear() {
	color_slider.Clear();
}