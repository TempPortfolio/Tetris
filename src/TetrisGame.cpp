#include "TetrisGame.h"

#include <cassert>
#include <iostream>
#include <random>

TetrisGame::TetrisGame()
{
	if (!font.loadFromFile("Pixeboy-z8XGD.ttf"))
	{
		std::cout << "Impossible to load font\n";
		assert(false);
	}

	setRandomTetrimino();
}

void TetrisGame::update()
{
	if (isGameOver)
	{
		gameOverUpdate();
		return;
	}

	static sf::Clock inputClk;
	static sf::Clock fallClk;

	sf::Time elapsed = inputClk.getElapsedTime();
	if (elapsed.asMilliseconds() >= 110)
	{
		inputClk.restart();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			tryRotate(false);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			if (tryFall())
				addScore(SCORE_DOWN);

		Vector2 toAdd;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			toAdd = Vector2(-1.f, 0.f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			toAdd = Vector2(1.f, 0.f);

		if (toAdd != Vector2(0.f, 0.f))
			if (!hasCollision(tPosition + toAdd, true, false, true, false, true))
				tPosition = tPosition + toAdd;
	}

	elapsed = fallClk.getElapsedTime();
	if (elapsed.asMilliseconds() >= fallSpeed)
	{
		fallClk.restart();

		if (!tryFall())
		{
			if (tPosition.y < 0.f)
			{
				isGameOver = true;
			}
			else
			{
				for (int x = 0; x < tsize; x++)
					for (int y = 0; y < tsize; y++)
					{
						Block b = tfalling[x][y];
						if (!b.isNull())
						{
							Vector2 place = Vector2(x, y) + tPosition;
							gride[place.x][place.y] = b;
						}
					}
				clearFilledLines();
				setRandomTetrimino();
			}
		}
	}
}

void TetrisGame::gameOverUpdate()
{
	static bool beginStop = true;
	static sf::Clock restartClk;

	if (beginStop)
	{
		beginStop = false;
		restartClk.restart();
		return;
	}

	sf::Time elapsed = restartClk.getElapsedTime();
	if (elapsed.asMilliseconds() >= restartWait)
	{
		beginStop = true;
		isGameOver = false;
		score = 0;

		clearAllLines();
		setRandomTetrimino();
	}
}

void TetrisGame::clearAllLines() 
{
	for (int x = 0; x < G_WIDTH; x++)
		for (int y = 0; y < G_HEIGTH; y++)
			gride[x][y] = Block::VOID;
}

void TetrisGame::clearFilledLines()
{
	int nbFilledLines = 0;
	for (int line = 0; line < G_HEIGTH; line++)
	{
		bool isFilledLine = true;
		for (int x = 0; x < G_WIDTH; x++)
		{
			Color c = gride[x][line];
			if (c.isNull())
			{
				isFilledLine = false;
				break;
			}
		}
		if (!isFilledLine)
			continue;
		
		nbFilledLines++;

		int start = line;
		for(int y = start - 1; y > 0; y--)
			for (int x = 0; x < G_WIDTH; x++)
			{
				int befor = y + 1;
				gride[x][befor] = gride[x][y];
			}
	}

	if (nbFilledLines != 0)
	{
		int toAdd = nbFilledLines * SCORE_LINE;
		if (nbFilledLines > 1)
			toAdd += nbFilledLines * SCORE_LINE_ADD;
		addScore(toAdd);
	}
}

bool TetrisGame::tryFall()
{
	Vector2 newPos = tPosition + Vector2(0, 1);
	if (!hasCollision(newPos, true, false, false, true, false))
	{
		tPosition = newPos;
		return true;
	}
	return false;
}

bool TetrisGame::tryRotate(bool right)
{
	int size = tsize, max = size - 1;

	Gride& shape = tfalling;
	Gride rotation = shape;

	for (int x = 0; x < size; x++)
	{
		int ny = right ? max - x : x;
		for (int y = 0; y < size; y++)
		{
			int nx = right ? y : max - y;
			rotation[nx][ny] = shape[x][y];
		}
	}
	if (!hasCollision(tPosition, true, false, true, true, true, rotation))
	{
		tfalling = rotation;
		return true;
	}

	return false;
}

bool TetrisGame::hasCollision(Vector2 tpos, bool blocks, bool bup, bool bright, bool bdown, bool bleft)
{
	return hasCollision(tpos, blocks, bup, bright, bdown, bleft, tfalling);
}
bool TetrisGame::hasCollision(Vector2 tpos, bool blocks, bool bup, bool bright, bool bdown, bool bleft, Gride& cgride)
{
	for (int x = 0; x < tsize; x++)
		for (int y = 0; y < tsize; y++)
		{
			if (cgride[x][y].isNull())
				continue;

			Vector2 pos = Vector2(x, y) + tpos;
			if ((bup && pos.y < 0) || (bdown && pos.y > G_HEIGTH - 1) || (bleft && pos.x < 0) || (bright && pos.x > G_WIDTH - 1))
				return true;

			if(blocks)
				for (int gx = 0; gx < G_WIDTH; gx++)
					for (int gy = 0; gy < G_HEIGTH; gy++)
						if (!gride[gx][gy].isNull() && gx == pos.x && gy == pos.y)
							return true;
		}

	return false;
}

void TetrisGame::draw(sf::RenderWindow* window)
{
	window->clear(sf::Color(0, 0, 0));

	for (int x = 0; x < G_WIDTH; x++)
		for (int y = 0; y < G_HEIGTH; y++)
		{
			Block b = gride[x][y];

			if (!b.isNull());
			else if (y % 2 != 0)
				b = Block(128, 128, 128);
			else
				b = Block(105, 105, 105);

			drawBlock(window, (float)x, (float)y, b);
		}
	
	sf::Text text;
	text.setFont(font);
	text.setString(std::string("Score: ") + std::to_string(score));
	text.setCharacterSize(60);
	text.setFillColor(sf::Color(255, 128, 0));

	text.setPosition(sf::Vector2f((float)G_WIDTH * scale + 20.f, 0.f));
	window->draw(text);

	if (isGameOver)
		drawGameOver(window);
	
	if (tIsFalling)
	{
		for (int x = 0; x < tsize; x++)
			for (int y = 0; y < tsize; y++)
				drawBlock(window, (float)x + tPosition.x, (float)y + tPosition.y, tfalling[x][y]);

	}

	window->display();
}

void TetrisGame::drawGameOver(sf::RenderWindow* window)
{
	static sf::Clock textClk;
	sf::Int32 freq = 500;
	sf::Int32 curr = textClk.getElapsedTime().asMilliseconds();

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(60);

	if (curr < freq)
	{
		text.setString("Game Over");
		text.setFillColor(sf::Color(255, 0, 0));

		text.setPosition(sf::Vector2f((float)G_WIDTH * scale + 20.f, 60.f));
		window->draw(text);
	}
	else if (curr >= freq * 2)
		textClk.restart();

	text.setString("Game will");
	text.setFillColor(sf::Color(255, 0, 125));

	text.setPosition(sf::Vector2f((float)G_WIDTH * scale + 20.f, 120.f));
	window->draw(text);

	text.setString("restart");

	text.setPosition(sf::Vector2f((float)G_WIDTH * scale + 20.f, 180.f));
	window->draw(text);
}

void TetrisGame::setRandomTetrimino()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(0, tetriminosSize - 1);

	Tetrimino& newTetr = tetriminos[dist(gen)];
	tsize = newTetr.size;
	tfalling = newTetr.blocks;
	tPosition = Vector2(4.f, (float)tsize * -1.f);
}

void TetrisGame::drawBlock(sf::RenderWindow* wnd, float x, float y, Block color)
{
	static sf::RectangleShape rect;

	Vector2 scaleVec(scale, scale);
	Vector2 pos = (Vector2(x, y) + origin) * scaleVec;

	rect.setFillColor(color.toSFML());
	rect.setPosition(pos.toSFML());
	rect.setSize(scaleVec.toSFML());

	wnd->draw(rect);
}

void TetrisGame::addScore(int toAdd)
{
	score += toAdd;
}