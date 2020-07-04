#pragma once

struct Vec2
{
	Vec2()
		: x(0.0f)
		, y(0.0f)
	{}

	Vec2(float x_, float y_)
		: x(x_)
		, y(y_)
	{}

	float x, y;

	void operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
	}

	float getLength() const
	{
		return sqrt(x*x + y*y);
	}

	Vec2 getNormalized() const
	{
		const float length = getLength();
		return Vec2(x / length, y / length);
	}
};


Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const Vec2& v, float f)
{
	return Vec2(v.x * f, v.y * f);
}

Vec2 operator*(float f, const Vec2& v)
{
	return Vec2(v.x * f, v.y * f);
}





