#include "SpriteButton.h"

SpriteButton::SpriteButton() { front_shade = olc::BLANK; }

SpriteButton::SpriteButton(const std::string& name, int id, const olc::vi2d& p, const olc::vi2d& s) 
	: name(name), id(id), pos(p), size(s) {
	front_shade = olc::BLANK;
}

void SpriteButton::Initialize(const std::string& name, int id, const olc::vi2d& p, const olc::vi2d& s) {
	this->id = id;
	this->name = name;
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
		is_hover = false;
		OnMouseRelease();
	}
}

bool SpriteButton::GetIsHover() const {
	return is_hover;
}

void SpriteButton::OnMousePress() {
	on_press = true;
	front_shade = olc::Pixel(255, 255, 255, 100);
}

void SpriteButton::OnMouseHover() {
	front_shade = olc::Pixel(150, 150, 150, 100);
	is_hover = true;
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

void SpriteButton::RenderName(const olc::vi2d& p, olc::PixelGameEngine* pge, olc::Pixel text_color, int scale) {
	if (is_hover) pge->DrawStringDecal((olc::vf2d)pos + (olc::vf2d)p, name, text_color, { (float)scale, (float)scale });
}

std::string SpriteButton::GetName() const {
	return name;
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