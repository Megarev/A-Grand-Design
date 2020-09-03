#pragma once
#include "olcPixelGameEngine.h"

class ToggleButton {
private:
	olc::Sprite* sprite_sheet = nullptr;
	olc::Decal* decal = nullptr;
	olc::Pixel front_shade;
	olc::vi2d pos, size;
	int id;

	bool on_press, is_pressed, is_toggle;
public:
	ToggleButton();
	ToggleButton(const std::string& filepath, int id, const olc::vi2d& p, const olc::vi2d& s);

	void Initialize(int id, const olc::vi2d& p, const olc::vi2d& s);

	bool IsPointInBounds(const olc::vi2d& point);

	void Logic(olc::PixelGameEngine* pge);

	void OnMousePress();
	void OnMouseHover();
	void OnMouseRelease();

	bool GetIsPressed();
	bool GetIsToggled() const;
	int GetID() const;

	olc::vi2d GetPosition() const;
	olc::vi2d GetSize() const;

	void Render(int index, olc::PixelGameEngine* pge);
	void Clear();
};
