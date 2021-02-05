#pragma once

#include "solver.hpp"

struct Grass
{
	Grass()
	{
		color = sf::Color(0, 100 + rand()%100, 0);
	}

	static Grass add(Solver& solver, float x, float y)
	{
		Grass grass;

		float strength = 0.0125f;
		float link_length = 70.0f + rand() % 70;
		const uint32_t points_count = 7;
		VerletPoint::ptr last_point = solver.createPoint(x, y, 1.0f, false);
		grass.points.push_back(last_point);
		Link::ptr last_link = nullptr;
		for (uint32_t i(0); i < points_count; ++i) {
			VerletPoint::ptr new_point = solver.createPoint(last_point->coords.x, last_point->coords.y - link_length, 0.001f / float(i + 1), i > 0);
			grass.points.push_back(new_point);
			Link::ptr new_link = solver.createLink(last_point, new_point);
			last_point = new_point;
			if (i > 0) {
				solver.createJoin(last_link, new_link, getRandRange(0.2f), strength);
				strength *= 2.0f;
			}
			link_length *= 0.8f;
			last_link = new_link;
		}

		return grass;
	}

	void addToVa(sf::VertexArray& va)
	{
		const float initial_width = 4.0f;
		float width = initial_width;
		const uint64_t points_count = points.size();
		for (uint32_t i(0); i < points_count - 1; ++i) {
			const Vec2& current_pt = points[i]->coords;
			const Vec2& next_pt    = points[i + 1]->coords;
			const Vec2 v = (next_pt - current_pt).getNormalized();

			sf::Vertex v1(sf::Vector2f(current_pt.x - width * v.y, current_pt.y + width * v.x), color);
			va.append(v1);
			sf::Vertex v2(sf::Vector2f(current_pt.x + width * v.y, current_pt.y - width * v.x), color);
			va.append(v2);

			width *= 0.5f;
			sf::Vertex v3(sf::Vector2f(next_pt.x + width*v.y, next_pt.y - width * v.x), color);
			va.append(v3);
			sf::Vertex v4(sf::Vector2f(next_pt.x - width * v.y, next_pt.y + width * v.x), color);
			va.append(v4);
		}
	}

	sf::Color color;
	std::vector<VerletPoint::ptr> points;
};

