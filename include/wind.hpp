#pragma once
#include "solver.hpp"


struct Wind
{
	float wind_x = 0.0f;
	float wind_speed = 7.0f;
	float wind_width = 200.0f;
	float wind_force = 50.0f;

	Wind()
	{
		reset();
	}

	void apply(Solver& solver) {
		for (VerletPoint::ptr pt : solver.getPoints()) {
			if (pt->coords.x > wind_x && pt->coords.x < wind_x + wind_width) {
				pt->acceleration += Vec2(wind_force, 0.0f);
			}
		}
	}

	void update(float win_width)
	{
		wind_x += wind_speed;
		if (wind_x > win_width + wind_width * 0.5f) {
			reset();
		}
	}

	void reset()
	{
		wind_width = 10.0f + getRandUnder(200.0f);
		wind_x = -wind_width * 0.5f;
		wind_force = 2.0f + getRandUnder(2.0f);
		wind_speed = 5.0f + getRandUnder(2.0f);
	}
};

