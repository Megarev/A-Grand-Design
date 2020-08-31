#pragma once
#include "olcPixelGameEngine.h"
#include <random>

struct Particle {
	olc::vf2d position, velocity, acceleration;
	olc::Pixel color;
	float size;
	int alphaSpeed;

	bool GetIsValue() {
		return (color.a > 0);
	}

	void Update(float dt) {
		velocity += acceleration;
		position += velocity;
		color.a -= alphaSpeed;
	}
};


class ParticleSystem {
private:
	std::vector<Particle> particles;
	int nParticleMax;
public:
	enum ParticleTypes {
		CircleRain,
		Squares,
		Circles,
		Circles2
	};

	ParticleSystem() { nParticleMax = 200; }

	void InitializeParticle(const Particle& particle) {
		if ((int)particles.size() > nParticleMax) return;
		particles.push_back(particle);
	}

	void Update(float dt) {
		Logic();
		for (auto it = particles.begin(); it != particles.end();) {
			it->Update(dt);

			if (!it->GetIsValue()) {
				it = particles.erase(it);
			}
			else {
				it++;
			}
		}
	}

	virtual void Logic() {}
	virtual void Input(float x, float y, const olc::Pixel& color) {}
	virtual void Render(olc::PixelGameEngine* pge) = 0;

	std::vector<Particle>& GetParticles() { return particles; }
};

/*
float RandomRange(float r1, float r2) {
	std::random_device rd;
	std::mt19937 m(rd());
	std::uniform_real_distribution<float> value(r1, r2);

	return value(m);
}

class CircleRainParticleSystem : public ParticleSystem {
public:
	void Input(float x, float y, const olc::Pixel& color) override {
		Particle particle = {
			{ x, y },
			{ RandomRange(-0.2f, 0.2f), RandomRange(0.0f, 0.05f) },
			{ 0.0f, RandomRange(0.0f, 0.005f) },
			color,
			0.0f,
			1
		};

		InitializeParticle(particle);
	}

	void Render(olc::PixelGameEngine* pge) override {
		for (const auto& p : GetParticles()) {
			pge->FillCircle(p.position, 2, p.color);
		}
	}
};

class SquareParticleSystem : public ParticleSystem {
private:
	float size, angle;
	std::vector<olc::vf2d> squareModel;

	void Fill_Rect(olc::PixelGameEngine* pge, const std::vector<olc::vf2d>& coords, const olc::Pixel& color) {

		if (coords.size() < 4) return;

		pge->FillTriangle(coords[0], coords[1], coords[2], color);
		pge->FillTriangle(coords[0], coords[3], coords[2], color);
	}

	void DrawModel(olc::PixelGameEngine* pge, const olc::vf2d& position, const olc::vf2d& origin, float rotation, olc::Pixel color = olc::WHITE) {
		std::vector<olc::vf2d> newCoords(4);

		// Rotation
		for (int i = 0; i < (int)newCoords.size(); i++) {
			newCoords[i].x = position.x + (squareModel[i].x + origin.x) * cosf(rotation) - (squareModel[i].y + origin.y) * sinf(rotation);
			newCoords[i].y = position.y + (squareModel[i].x + origin.x) * sinf(rotation) + (squareModel[i].y + origin.y) * cosf(rotation);
		}

		Fill_Rect(pge, newCoords, color);
	}
public:
	SquareParticleSystem() {
		angle = 0.0f;
		size = 4.0f;

		squareModel = {
			{ 0.0f, 0.0f },
			{ size, 0.0f },
			{ size, size },
			{ 0.0f, size }
		};
	}

	void Input(float x, float y, const olc::Pixel& color) override {
		Particle particle = {
			{ x, y },
			{ RandomRange(-0.2f, 0.2f), RandomRange(0.0f, 0.05f) },
			{ 0.0f, RandomRange(0.0f, 0.005f) },
			color,
			0.0f,
			1
		};

		InitializeParticle(particle);
	}

	void Logic() override {
		angle++;
	}

	void Render(olc::PixelGameEngine* pge) override {
		for (const auto& p : GetParticles()) {
			DrawModel(pge, p.position, { -size / 2.0f, -size / 2.0f }, angle, p.color);
		}
	}
};*/

class CircleParticleSystem : public ParticleSystem {
private:
	float radius;
public:
	CircleParticleSystem() {
		radius = 0.0f;
	}

	void Input(float x, float y, const olc::Pixel& color) override {
		Particle particle = {
			{ x, y },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			color,
			0.0f,
			1
		};

		InitializeParticle(particle);
	}

	void Logic() override {
		for (auto& p : GetParticles()) p.size += 0.5f;
	}

	void Render(olc::PixelGameEngine* pge) override {
		for (const auto& p : GetParticles()) {
			pge->FillCircle(p.position, (int)p.size, p.color);
		}
	}
};

/*
class Circle2ParticleSystem : public ParticleSystem {
private:
	std::vector<Particle> squareParticle;
public:
	void Input(float x, float y, const olc::Pixel& color) override {

		//const olc::Pixel& color = olc::Pixel(100 + rand() % 156, 100 + rand() % 156, 100 + rand() % 156, 100 + rand() % 156);

		for (int i = 0; i < 10; i++) {
			Particle p = {
				{ x, y },
				{ RandomRange(-0.2f, 0.2f), RandomRange(-0.3f, 0.1f) },
				{ 0.0f, RandomRange(0.0f, 0.005f) },
				color,
				RandomRange(1.0f, 4.0f)
			};

			squareParticle.push_back(p);
		}

		Particle particle = {
			{ x, y },
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			olc::Pixel(color.r, color.g, color.b, 156),
			25.0f,
			1
		};

		InitializeParticle(particle);
	}

	void Logic() override {
		for (auto it = squareParticle.begin(); it != squareParticle.end();) {
			it->Update(1.0f);

			if (!it->GetIsValue())
				it = squareParticle.erase(it);
			else
				it++;
		}
	}

	void Render(olc::PixelGameEngine* pge) override {
		for (const auto& p : GetParticles()) {
			pge->FillCircle(p.position, (int)p.size, p.color);
		}

		for (const auto& s : squareParticle) {
			pge->FillRect(s.position, { (int)s.size, (int)s.size }, s.color);
		}
	}
};
*/