#include "SpriteButton.h"

SpriteButton::SpriteButton() { front_shade = olc::BLANK; }

SpriteButton::SpriteButton(int id, const olc::vi2d& p, const olc::vi2d& s) 
	: id(id), pos(p), size(s) {
	front_shade = olc::BLANK;
}

void SpriteButton::Initialize(int id, const olc::vi2d& p, const olc::vi2d& s) {
	this->id = id;
	pos = p;
	size = s;
}

bool SpriteButton::IsPointInBounds(const olc::vi2d& point) {
	return (point.x > pos.x && point.x < pos.x + size.x && point.y > pos.y && point.y < pos.y + size.y);
}

void SpriteButton::Logic(olc::PixelGameEngine* pge) {
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

void SpriteButton::OnMousePress() {
	on_press = true;
	front_shade = olc::Pixel(255, 255, 255, 100);
}

void SpriteButton::OnMouseHover() {
	front_shade = olc::Pixel(150, 150, 150, 100);
}

void SpriteButton::OnMouseRelease() {
	front_shade = olc::BLANK;
	on_press = false;
}

bool SpriteButton::GetIsPressed() {
	if (!on_press) is_pressed = false;

	if (on_press && !is_pressed) {
		is_pressed = true;
		return true;
	}

	return false;
}

int SpriteButton::GetID() const {
	return id;
}

olc::vi2d SpriteButton::GetPosition() const {
	return pos;
}

olc::vi2d SpriteButton::GetSize() const {
	return size;
}

void SpriteButton::GetSpriteSheet(olc::Sprite* sprite) {
	sprite_sheet = sprite;
}

void SpriteButton::Render(int index, olc::PixelGameEngine* pge, int index_y) {
	
	if (sprite_sheet == nullptr) return;

	// Drawing the sprite
	pge->DrawPartialSprite({ pos.x + 1, pos.y }, sprite_sheet, { index * size.x, index_y * size.y }, { size.x, size.y });

	// Drawing the front shade
	pge->SetPixelMode(olc::Pixel::ALPHA);
	pge->FillRectDecal({ (float)pos.x + 1, (float)pos.y }, size, front_shade);
	pge->SetPixelMode(olc::Pixel::NORMAL);
}