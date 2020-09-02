#pragma once
#include "SpriteButton.h"
#include "olcPixelGameEngine.h"

class GUI {
private:
	std::vector<SpriteButton> buttons;
	olc::Sprite* frame_UI;

	olc::Pixel selected_button;
	olc::vi2d selected_button_pos;
	int n_units, psize, selected_index;
public:
	GUI();
	void SetSpriteSheet(olc::Sprite* spritesheet);
	void Logic(olc::PixelGameEngine* pge);
	void Render(olc::PixelGameEngine* pge);

	int GetSelectedIndex() const;
	void Clear();
};