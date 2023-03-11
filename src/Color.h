#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include <SFML/Graphics.hpp>

class Color
{
public:
	Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
	uint8_t r, g, b, a;

	sf::Color toSFML();

	bool isNull();

	bool operator==(const Color& color);
	bool operator!=(const Color& color);

	static const Color VOID;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};

#endif