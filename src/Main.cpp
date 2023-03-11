#include <iostream>
#include <SFML/Graphics.hpp>
#include "TetrisGame.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(525, 550), "Tetris");
	window.setFramerateLimit(30);

	TetrisGame game;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
		}

		game.update();
		game.draw(&window);
	}

	return 0;
}