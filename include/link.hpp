#pragma once

#include <memory>
#include "verlet_point.hpp"
#include "utils.hpp"


struct Link
{
	using ptr = std::shared_ptr<Link>;

	Link()
		: point1(nullptr)
		, point2(nullptr)
		, strength(1.0f)
	{}

	Link(VerletPoint::ptr p1, VerletPoint::ptr p2)
		: point1(p1)
		, point2(p2)
		, strength(1.0f)
	{
		length = getLength(*p1 - *p2);
	}

	Vec2 getVec() const
	{
		return (point2->coords - point1->coords).getNormalized();
	}

	Vec2 update()
	{
		const Vec2 v = *point1 - *point2;
		const Vec2 v_normalized = v.getNormalized();
		const float current_length = getLength(v);
		const float delta = length - current_length;

		const float amp_factor = !point1->moving || !point2->moving ? 1.0f : 0.5f;
		const float amp = 1.0f * amp_factor * delta * strength;

		const float mass_total = point1->mass + point2->mass;
		const float mass_coef1 = point2->mass / mass_total;
		const float mass_coef2 = point1->mass / mass_total;

		const Vec2 offset = v_normalized * mass_coef2 * -amp;
		point1->move(v_normalized * mass_coef1 *  amp);
		point2->move(offset);

		return offset;
	}

	static ptr create(VerletPoint::ptr pt1, VerletPoint::ptr pt2)
	{
		return std::make_shared<Link>(pt1, pt2);
	}

	VerletPoint::ptr point1;
	VerletPoint::ptr point2;
	float length;
	float strength;
};
