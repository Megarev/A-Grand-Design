#include "UI.h"

GUI::GUI() {
	psize = 20;
	n_units = 6;

	frame_UI = new olc::Sprite("files/images/FrameUI.png");

	selected_button = olc::WHITE;
	selected_button_pos = { 0, psize };

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
}

void GUI::Render(olc::PixelGameEngine* pge) {

	pge->SetPixelMode(olc::Pixel::ALPHA);
	pge->DrawSprite(0, 0, frame_UI);
	pge->SetPixelMode(olc::Pixel::NORMAL);

	for (int i = 0; i < n_units; i++) {
		buttons[i].Render(i, pge);
	}

	pge->DrawRect({ 0, psize * selected_index }, { psize, psize });
}

int GUI::GetSelectedIndex() const {
	return selected_index;
}

void GUI::Clear() {
	delete frame_UI;
}