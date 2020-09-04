#pragma once
#include "SpriteButton.h"
#include "ToggleButton.h"
#include "UnitManager.h"
#include "Slider.h"
#include "olcPixelGameEngine.h"

class GUI {
private:
	std::vector<SpriteButton> buttons;
	ToggleButton run_button;
	SpriteButton clear_button;
	Slider color_slider;

	UnitManager* unit_mgr;
	std::string button_names[6] = { "Static Unit", "Propeller unit", "Reverser unit", "Wall unit", "Gravity unit", "Generator unit" };

	olc::Pixel selected_button, border_color;
	olc::vi2d selected_button_pos;
	int n_units, psize, selected_index;
public:
	GUI();
	GUI(int screen_width, int screen_height);
	void SetSpriteSheet(olc::Sprite* spritesheet);
	void Logic(olc::PixelGameEngine* pge);
	void Render(olc::PixelGameEngine* pge);

	void GetUnitManager(UnitManager* u);
	bool ClearUnits();
	bool Run();

	olc::Pixel GetColorFromSlider() const;
	int GetSelectedIndex() const;
	void Clear();
};