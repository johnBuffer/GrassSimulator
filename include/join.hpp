#pragma once

#include "link.hpp"


struct Join
{
	using ptr = std::shared_ptr<Join>;

	Join(Link::ptr link_1_, Link::ptr link_2_, float target_angle_, float strength_)
		: target_angle(target_angle_)
		, strength(strength_)
		, last_delta(0.0f)
	{
		if (&(*link_1_->point1) == &(*link_2_->point1)) {
			setPoints(link_1_->point2, link_1_->point1, link_2_->point2);
		}
		else if (&(*link_1_->point1) == &(*link_2_->point2)) {
			setPoints(link_1_->point2, link_1_->point1, link_2_->point1);
		}
		else if (&(*link_1_->point2) == &(*link_2_->point1)) {
			setPoints(link_1_->point1, link_1_->point2, link_2_->point2);
		}
		else {
			setPoints(link_1_->point1, link_1_->point2, link_2_->point1);
		}
	}

	void setPoints(VerletPoint::ptr pt_1_, VerletPoint::ptr pt_2_, VerletPoint::ptr pt_3_)
	{
		pt_1 = pt_1_;
		pt_2 = pt_2_;
		pt_3 = pt_3_;

		link_length_1 = (pt_1->coords - pt_2->coords).getLength();
		link_length_2 = (pt_3->coords - pt_2->coords).getLength();
	}

	void update(float dt)
	{
		const Vec2 v1 = pt_2->coords - pt_1->coords;
		const Vec2 v2 = pt_3->coords - pt_2->coords;

		const float length = v2.getLength();
		const Vec2 normalized_v2 = v2.getNormalized() * link_length_2;

		const float current_angle = getVec2Angle(v1, v2);
		const float delta_angle = (target_angle - current_angle);
		// Enable damages
		if (std::abs(delta_angle) > 0.5f * PI) {
			target_angle -= sign(delta_angle) * 0.025f;
		}
		
		const float cos_a = cos(0.5f * strength * delta_angle);
		const float sin_a = sin(0.5f * strength * delta_angle);
		const float d_x = normalized_v2.x * cos_a - normalized_v2.y * sin_a;
		const float d_y = normalized_v2.x * sin_a + normalized_v2.y * cos_a;

		pt_3->moveTo(pt_2->coords + Vec2(d_x, d_y));
		for (Join::ptr join : sub_joins) {
			join->rotate(cos_a, sin_a, pt_2->coords);
		}

		last_delta = delta_angle;
		
	}

	void addSub(Join::ptr join)
	{
		sub_joins.push_back(join);
	}

	void rotate(float ca, float sa, const Vec2& origin)
	{
		const Vec2 v = pt_3->coords - origin;
		const float d_x = v.x * ca - v.y * sa;
		const float d_y = v.x * sa + v.y * ca;

		pt_3->moveTo(origin + Vec2(d_x, d_y));

		for (Join::ptr join : sub_joins) {
			join->rotate(ca, sa, origin);
		}
	}

	template<typename... Args>
	static ptr create(Args&&... args)
	{
		return std::make_shared<Join>(args...);
	}

	float strength;
	float target_angle;
	float last_delta;

	float link_length_1;
	float link_length_2;

	std::vector<Join::ptr> sub_joins;

	VerletPoint::ptr pt_1;
	VerletPoint::ptr pt_2;
	VerletPoint::ptr pt_3;
};

