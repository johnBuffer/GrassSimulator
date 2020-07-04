#pragma once

#include "vec2.hpp"
#include <memory>


struct VerletPoint
{
	using ptr = std::shared_ptr<VerletPoint>;

	VerletPoint()
		: coords()
		, last_coords()
		, acceleration()
		, mass(1.0f)
		, moving(true)
	{}

	VerletPoint(float x, float y, float mass_)
		: coords(x, y)
		, last_coords(x, y)
		, acceleration()
		, mass(mass_)
		, moving(true)
	{}

	void rollback()
	{
		coords = last_coords;
		acceleration = Vec2(0.0f, 0.0f);
	}

	void moveTo(float x, float y, bool bypass = false)
	{
		if (moving || bypass) {
			coords.x = x;
			coords.y = y;
		}
	}

	void moveTo(const Vec2& p, bool bypass = false)
	{
		if (moving || bypass) {
			coords.x = p.x;
			coords.y = p.y;
		}
	}

	void move(float x, float y)
	{
		if (moving) {
			coords.x += x;
			coords.y += y;
		}
	}

	void move(const Vec2& p)
	{
		if (moving) {
			coords.x += p.x;
			coords.y += p.y;
		}
	}

	void moveAbs(const Vec2& p)
	{
		if (moving) {
			coords.x += p.x;
			coords.y += p.y;

			last_coords.x += p.x;
			last_coords.y += p.y;
		}
	}

	void stop()
	{
		last_coords = coords;
	}

	void update(float dt)
	{
		if (moving) {
			const Vec2 v = coords - last_coords;
			// Air friction
			acceleration = acceleration -v * 1.0f;
			last_coords = coords;
			coords += v + acceleration * (dt * dt / mass);
			acceleration = Vec2(0.0f, 0.0f);
		}
	}

	static ptr create(float x, float y, float mass)
	{
		return std::make_shared<VerletPoint>(x, y, mass);
	}

	Vec2 coords;
	Vec2 last_coords;
	Vec2 acceleration;

	float mass;
	bool moving;
};


Vec2 operator-(const VerletPoint& p1, const VerletPoint& p2)
{
	return p1.coords - p2.coords;
}
