#include "Slider.h"

Slider::Slider() {}

Slider::Slider(const olc::vi2d& p, int type) 
	: pos(p), orientation_type(type) {
	slider_sprite = new olc::Sprite("files/images/colors.png");
	size = { slider_sprite->width, slider_sprite->height };

	current_color = olc::WHITE;

	is_pressed = false;
}

bool Slider::IsPointInSprite(const olc::vi2d& point) {
	return (point.x > pos.x && point.x < pos.x + size.x && point.y > pos.y && point.y < pos.y + size.y);
}

void Slider::Input(olc::PixelGameEngine* pge) {
	const olc::vi2d& mouse_pos = olc::vi2d(pge->GetMouseX(), pge->GetMouseY());

	if (IsPointInSprite(mouse_pos)) {
		if (pge->GetMouse(0).bHeld) {
			is_pressed = true;
		}
	}

	if (is_pressed) slider_pointer = mouse_pos;
	if (pge->GetMouse(0).bReleased) is_pressed = false;
}

void Slider::Logic() {
	switch (orientation_type) {
	case 0: 
		slider_pointer.y = pos.y; 
		if (slider_pointer.x < pos.x) slider_pointer.x = pos.x;
		else if (slider_pointer.x > pos.x + size.x) slider_pointer.x = pos.x + size.x;
		break;
	case 1: 
		slider_pointer.x = pos.x; 
		if (slider_pointer.y < pos.y) slider_pointer.y = pos.y;
		else if (slider_pointer.y > pos.y + size.y) slider_pointer.y = pos.y + size.y;
		break;
	}

	if (is_pressed) {
		current_color = slider_sprite->GetPixel(1, slider_pointer.y - pos.y);
	}
}

void Slider::Render(olc::PixelGameEngine* pge) {
	pge->DrawSprite(pos.x, pos.y, slider_sprite);
	//pge->DrawLine(slider_pointer, { slider_pointer.x + size.x, slider_pointer.y }, olc::Pixel(200, 200, 200));
	pge->FillRect(slider_pointer - olc::vi2d(0, 1), { size.x, 3 }, olc::Pixel(200, 200, 200));
}

olc::Pixel Slider::GetColor() const {
	return current_color;
}

void Slider::Clear() {
	delete slider_sprite;
}