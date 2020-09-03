#pragma once
#include "olcPixelGameEngine.h"

class Slider {
private:
	olc::Sprite* slider_sprite = nullptr;
	olc::vi2d pos, size;
	olc::vi2d slider_pointer;
	olc::Pixel current_color;

	bool is_pressed;
	int orientation_type;
public:
	Slider();
	Slider(const olc::vi2d& p, int type);

	void Input(olc::PixelGameEngine* pge);
	void Logic();

	bool IsPointInSprite(const olc::vi2d& point);

	void Render(olc::PixelGameEngine* pge);

	olc::Pixel GetColor() const;
	void Clear();
};