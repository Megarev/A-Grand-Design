#pragma once
#include "SpriteButton.h"
#include "Slider.h"
#include "olcPixelGameEngine.h"

class GUI {
private:
	std::vector<SpriteButton> buttons;
	Slider color_slider;

	olc::Pixel selected_button, border_color;
	olc::vi2d selected_button_pos;
	int n_units, psize, selected_index;
public:
	GUI();
	GUI(int screen_width, int screen_height);
	void SetSpriteSheet(olc::Sprite* spritesheet);
	void Logic(olc::PixelGameEngine* pge);
	void Render(olc::PixelGameEngine* pge);

	olc::Pixel GetColorFromSlider() const;
	int GetSelectedIndex() const;
	void Clear();
};