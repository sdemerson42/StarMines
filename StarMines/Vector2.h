#pragma once

struct Vector2
{
	Vector2() {}
	Vector2(float _x, float _y) noexcept :
	x{ _x }, y{ _y }
	{}
	float x{};
	float y{};
};
