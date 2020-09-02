#pragma once
#include "olcPixelGameEngine.h"

class SpriteButton {
private:
	olc::Sprite* sprite_sheet = nullptr;
	olc::Pixel front_shade;
	olc::vi2d pos, size;
	int id;

	bool on_press, is_pressed;
public:
	SpriteButton();
	SpriteButton(int id, const olc::vi2d& p, const olc::vi2d& s);

	void Initialize(int id, const olc::vi2d& p, const olc::vi2d& s);

	bool IsPointInBounds(const olc::vi2d& point);
	void GetSpriteSheet(olc::Sprite* sprite);

	void Logic(olc::PixelGameEngine* pge);

	void OnMousePress();
	void OnMouseHover();
	void OnMouseRelease();

	bool GetIsPressed();
	int GetID() const;

	olc::vi2d GetPosition() const;
	olc::vi2d GetSize() const;

	void Render(int index, olc::PixelGameEngine* pge);
};