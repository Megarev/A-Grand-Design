#pragma once
#include "olcPixelGameEngine.h"
#include "Unit.h"
#include "StaticUnit.h"
#include "KinematicUnit.h"
#include "RotatorUnit.h"
#include "WallUnit.h"
#include "GravityUnit.h"
#include "GeneratorUnit.h"
#include "ParticleSystem.h"

class UnitManager {
private:
	std::string level;
	std::vector<olc::Pixel> colors; // Colors of each unit
	std::vector<Unit*> units, units_generated; // For generator unit only
	std::unique_ptr<ParticleSystem> p_system;
	int n_selected_unit;

	olc::Pixel current_color; // For units

	enum Units {
		Static,
		Kinematic,
		Rotator,
		Wall,
		Gravity,
		Generator
	};

	olc::vi2d level_size;
	int pixel_size, n_units;
public:
	UnitManager();
	UnitManager(const olc::vi2d& size, int psize);

	void Input(olc::PixelGameEngine* pge, const olc::vi2d& mouse_pos);
	void InitializeUnits();

	void Clear();
	void ClearLevel();
	void Logic(int unit, float dt);

	char GetUnit(int x, int y) const;
	void SetUnit(int x, int y, char c);

	void GetColor(const olc::Pixel& color);

	void RenderParticles(olc::PixelGameEngine* pge);

	std::vector<Unit*> GetUnits() const;
	std::string GetLevel() const;
	Unit* GetUnitAtIndex(int x, int y) const;

	bool IsAUnitConnected(int x, int y, char unit) const;
	bool IsAUnitWithGravity(int x, int y) const;
	std::pair<int, bool> CheckUnitNeighbours(int x, int y, char unit) const;

	int GetConnectIndex(int x, int y) const;
	int GetSelectedIndex() const;

	olc::Pixel GetCurrentColor() const;
	void SetColor(const olc::Pixel& color);

	std::vector<olc::Pixel> GetUnitColors() const;
	olc::Pixel GetColor(int x, int y) const;
};