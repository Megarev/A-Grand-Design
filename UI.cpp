#include "UI.h"

GUI::GUI() {}

GUI::GUI(int screen_width, int screen_height) {
	psize = 20;
	n_units = 6;

	selected_button = olc::WHITE;
	selected_button_pos = { 0, psize };

	color_slider = Slider({ screen_width - psize + 1, psize }, 1);

	border_color = olc::Pixel(100, 10, 255);

	for (int i = 0; i < n_units; i++) {
		buttons.push_back(SpriteButton(i, { 0, i * psize }, { psize, psize }));
	}
}

void GUI::SetSpriteSheet(olc::Sprite* spritesheet) {
	for (auto& b : buttons) {
		b.GetSpriteSheet(spritesheet);
	}
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
}

void GUI::Render(olc::PixelGameEngine* pge) {

	// Border around the window

	pge->DrawRect(psize + 1, psize + 1, pge->ScreenWidth() - 2 * (psize + 1), pge->ScreenHeight() - 2 * (psize + 1), border_color);
	pge->FillRectDecal({ 1, (float)psize * selected_index }, { (float)psize, (float)psize }, olc::Pixel(255, 255, 255, 100));

	for (int i = 0; i < n_units; i++) {
		buttons[i].Render(i, pge);
	}

	color_slider.Render(pge);
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