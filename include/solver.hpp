#pragma once

#include <SFML/Graphics.hpp>
#include "link.hpp"
#include "join.hpp"
#include <vector>


// Create a "TypeMap" 


class Solver
{
public:
	Solver() = default;

	VerletPoint::ptr createPoint(float x, float y, float mass, bool moving = true)
	{
		VerletPoint::ptr pt = VerletPoint::create(x, y, mass);
		pt->moving = moving;
		m_points.push_back(pt);
		return pt;
	}

	Link::ptr createLink(VerletPoint::ptr pt1, VerletPoint::ptr pt2)
	{
		Link::ptr link = Link::create(pt1, pt2);
		m_links.push_back(link);
		return link;
	}

	template<typename... Args>
	Join::ptr createJoin(Args&&... args)
	{
		Join::ptr join = Join::create(args...);
		m_joins.push_back(join);
		return join;
	}

	void applyGravity()
	{
		for (VerletPoint::ptr pt : m_points) {
			pt->acceleration.y = 500.0f * pt->mass;
		}
	}

	void update(float dt = 0.016f)
	{
		applyGravity();

		const uint32_t iterations_count = 1u;
		for (uint8_t i(iterations_count); i--;) {
			for (Join::ptr join : m_joins) {
				join->update(dt);
			}
			
			/*for (Link::ptr link : m_links) {
				link->update();
			}*/

			for (VerletPoint::ptr pt : m_points) {
				pt->update(dt);
			}
		}
	}

	VerletPoint::ptr getPointAt(float x, float y)
	{
		const float dist_threshold = 20.0f;
		for (VerletPoint::ptr pt : m_points) {
			const float dist = (pt->coords - Vec2(x, y)).getLength();
			if (dist < dist_threshold) {
				return pt;
			}
		}

		return nullptr;
	}

	void applyForce(float radius, float x, float y)
	{
		for (VerletPoint::ptr pt : m_points) {
			const Vec2 v = pt->coords - Vec2(x, y);
			const float dist = v.getLength();
			const Vec2 normalized = v.getNormalized();
			if (dist < radius) {
				pt->move(0.01f * (radius - dist) * v);
			}
		}
	}

	void render(sf::RenderTarget& target)
	{
		sf::VertexArray links(sf::Lines, m_links.size() * 2);
		uint64_t link_i = 0;
		for (Link::ptr link : m_links) {
			links[2 * link_i + 0].position = sf::Vector2f(link->point1->coords.x, link->point1->coords.y);
			links[2 * link_i + 1].position = sf::Vector2f(link->point2->coords.x, link->point2->coords.y);

			links[2 * link_i + 0].color = sf::Color::Green;
			links[2 * link_i + 1].color = sf::Color::Green;
			++link_i;
		}
		target.draw(links);

		/*const float join_radius = 4.0f;
		for (const Join::ptr join : m_joins) {
			sf::CircleShape c(join_radius);
			c.setOrigin(join_radius, join_radius);
			c.setFillColor(sf::Color::Red);
			const VerletPoint::ptr pt = join->pt_2;
			c.setPosition(pt->coords.x, pt->coords.y);
			target.draw(c);
		}

		const float point_radius = 2.0f;
		for (const VerletPoint::ptr pt : m_points) {
			sf::CircleShape c(point_radius);
			c.setOrigin(point_radius, point_radius);
			c.setFillColor(sf::Color::Yellow);
			c.setPosition(pt->coords.x, pt->coords.y);
			target.draw(c);
		}*/
	}

	std::vector<VerletPoint::ptr>& getPoints()
	{
		return m_points;
	}

private:
	std::vector<VerletPoint::ptr> m_points;
	std::vector<Link::ptr> m_links;
	std::vector<Join::ptr> m_joins;
};

