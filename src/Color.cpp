#include "Color.h"

sf::Color Color::toSFML()
{
	return sf::Color(r, g, b, a);
}

bool Color::isNull()
{
	return a == 0;
}

bool Color::operator==(const Color& color)
{
	return color.r == r && color.g == g && color.b == b && color.a == a;
}

bool Color::operator!=(const Color& color)
{
	return !(*this == color);
}

const Color Color::VOID = Color(0, 0, 0, 0);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);