#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <SFML/Graphics.hpp>

struct Vector2
{
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float x, float y) : x(x), y(y) {}

	float x;
	float y;

	Vector2 operator+(const Vector2 vec)
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator-(const Vector2 vec)
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 operator*(const Vector2 vec)
	{
		return Vector2(x * vec.x, y * vec.y);
	}

	Vector2 operator*(const float mult)
	{
		return Vector2(x * mult, y * mult);
	}

	bool operator==(const Vector2 vec)
	{
		return x == vec.x && y == vec.y;
	}

	bool operator!=(const Vector2 vec)
	{
		return !(*this == vec);
	}

	sf::Vector2f toSFML()
	{
		return sf::Vector2f(x, y);
	}
};

#endif