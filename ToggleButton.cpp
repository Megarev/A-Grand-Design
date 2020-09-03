#include "ToggleButton.h"

ToggleButton::ToggleButton() {}

ToggleButton::ToggleButton(const std::string& filepath, int id, const olc::vi2d& p, const olc::vi2d& s) 
	: id(id), pos(p), size(s) {
	sprite_sheet = new olc::Sprite(filepath);
	decal = new olc::Decal(sprite_sheet);
	is_toggle = false;
	is_pressed = true;
}

void ToggleButton::Initialize(int id, const olc::vi2d& p, const olc::vi2d& s) {
	this->id = id;
	pos = p;
	size = s;
}

bool ToggleButton::IsPointInBounds(const olc::vi2d& point) {
	return (point.x > pos.x && point.x < pos.x + size.x && point.y > pos.y && point.y < pos.y + size.y);
}

void ToggleButton::Logic(olc::PixelGameEngine* pge) {
	const olc::vi2d& mouse_pos = olc::vi2d(pge->GetMouseX(), pge->GetMouseY());

	if (IsPointInBounds(mouse_pos)) {
		OnMouseHover();
		if (pge->GetMouse(0).bHeld) OnMousePress();
		else if (pge->GetMouse(0).bReleased) OnMouseRelease();
	}
	else {
		OnMouseRelease();
	}
}

void ToggleButton::OnMousePress() {
	on_press = true;
	front_shade = olc::Pixel(255, 255, 255, 100);
}

void ToggleButton::OnMouseHover() {
	front_shade = olc::Pixel(150, 150, 150, 100);
}

void ToggleButton::OnMouseRelease() {
	front_shade = olc::BLANK;
	on_press = false;
}

bool ToggleButton::GetIsPressed() {
	if (!on_press) is_pressed = false;

	if (on_press && !is_pressed) {
		is_pressed = true;
		is_toggle = !is_toggle;
		return true;
	}

	return false;
}

void ToggleButton::Render(int index, olc::PixelGameEngine* pge) {

	if (sprite_sheet == nullptr) return;

	std::cout << is_toggle << std::endl;

	pge->SetPixelMode(olc::Pixel::ALPHA);
	//pge->DrawPartialSprite(pos, sprite_sheet, { is_toggle * size.x, index * size.y }, size);
	pge->DrawPartialDecal((olc::vf2d)pos, (olc::vf2d)size, decal, { is_toggle * (float)size.x, index * (float)size.y }, (olc::vf2d)size);
	pge->FillRectDecal({ (float)pos.x, (float)pos.y }, size, front_shade);
	pge->SetPixelMode(olc::Pixel::NORMAL);
}

bool ToggleButton::GetIsToggled() const { return is_toggle; }

int ToggleButton::GetID() const { return id; }

olc::vi2d ToggleButton::GetPosition() const { return pos; }
olc::vi2d ToggleButton::GetSize() const { return size; }

void ToggleButton::Clear() {
	delete decal;
	delete sprite_sheet;
}