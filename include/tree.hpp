#pragma once

#include "solver.hpp"

const float branch_length = 250.0f;
const float branch_length_reduction = 0.7f;
const uint32_t max_depth = 6;
const uint32_t fork_count = 3;
const float fork_angle = 3.141592653f * 0.5f;


struct Branch
{
	VerletPoint::ptr connector;
	Link::ptr link;
	Join::ptr join;
};


struct Tree
{
	static void add(Solver& solver, float x, float y)
	{
		VerletPoint::ptr root = solver.createPoint(x, y, 1.0f, false);
		VerletPoint::ptr first = solver.createPoint(x, y - 10, 1.0f, false);
		Link::ptr link = solver.createLink(root, first);

		Branch first_branch;
		first_branch.connector = first;
		first_branch.link = link;
		first_branch.join = nullptr;

		addBranch(max_depth, first_branch, branch_length * branch_length_reduction, solver);
	}

	static void addBranch(uint32_t depth, Branch branch, float branch_length, Solver& solver)
	{
		if (!depth) {
			return;
		}

		const Vec2 v = branch.link->getVec();
		const Vec2 pt = branch.connector->coords + branch_length * v;
		const float angle = getRandRange(fork_angle * (depth != max_depth));
		const Vec2 next_pt = rotate(pt, branch.connector->coords, angle);
		
		float mass = 1.0f;
		if (depth == 1) {
			mass = 0.01f;
		}
		else {
			mass = 0.01f * float(1.0f * depth + 1.0f);
		}

		VerletPoint::ptr point = solver.createPoint(next_pt.x, next_pt.y, mass);
		Link::ptr link = solver.createLink(branch.connector, point);

		Join::ptr join = solver.createJoin(branch.link, link, angle, 0.01f * depth);
		if (branch.join) {
			branch.join->addSub(join);
		}

		Branch new_branch;
		new_branch.connector = point;
		new_branch.link = link;
		new_branch.join = join;

		for (uint8_t i(fork_count); i--;) {
			addBranch(depth - 1, new_branch, branch_length * branch_length_reduction, solver);
		}
	}
};
