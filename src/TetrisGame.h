#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include "Color.h"
#include "Vector2.h"

typedef Color Block;
typedef std::vector<std::vector<Block>> Gride;

static Gride cg(const std::string& pattern, size_t size, Color color) //String to gride
{
	Gride gride(size, std::vector<Block>(size));

	for (size_t x = 0; x < size; x++)
		for (size_t y = 0; y < size; y++)
			gride[x][y] = pattern.at(y * size + x) == 'x' ?
			color : Block::VOID;

	return gride;
}

struct Tetrimino
{
	Tetrimino(Gride blocks, int size) : blocks(blocks), size(size)
	{}
	Gride blocks;
	const int size;
};

class TetrisGame
{
public:
	TetrisGame();

	void update();
	void draw(sf::RenderWindow* window);

private:
	bool hasCollision(Vector2 tpos, bool blocks, bool bup, bool bright, bool bdown, bool bleft);
	bool hasCollision(Vector2 tpos, bool blocks, bool bup, bool bright, bool bdown, bool bleft, Gride& cgride);

	bool tryRotate(bool right);
	bool tryFall();

	void clearAllLines();
	void clearFilledLines();
	void setRandomTetrimino();

	void gameOverUpdate();

	void drawBlock(sf::RenderWindow* window, float x, float y, Block color);
	void drawGameOver(sf::RenderWindow* window);

	void addScore(int toAdd);

private:
	const int G_WIDTH = 10, G_HEIGTH = 22;
	//const Block BG = Block(128, 128, 128);
	float scale = 25.f;
	Vector2 origin;
	Gride gride = Gride(G_WIDTH, std::vector<Block>(G_HEIGTH, Block::VOID));

	long fallSpeed = 500l;

	int score = 0;
	const int SCORE_DOWN = 1;
	const int SCORE_LINE = 10;
	const int SCORE_LINE_ADD = 5;

	Gride tfalling;
	int tsize = 0;
	bool tIsFalling = true;
	Vector2 tPosition;

	bool isGameOver = false;
	long restartWait = 5000;

	sf::Font font;

	static const int tetriminosSize = 7;
	Tetrimino tetriminos[tetriminosSize]
	{
		Tetrimino(cg("ooooooooxxxxoooo", 4, Block(0, 255, 255)), 4),
		Tetrimino(cg("oooxxxoox", 3, Block(0, 0, 255)), 3),
		Tetrimino(cg("oooxxxxoo", 3, Block(255, 165, 0)), 3),
		Tetrimino(cg("ooooxxxxo", 3, Block(0, 255, 0)), 3),
		Tetrimino(cg("oooxxxoxo", 3, Block(128, 0, 128)), 3),
		Tetrimino(cg("oooxxooxx", 3, Block(255, 0, 0)), 3),
		Tetrimino(cg("xxxx", 2, Block(255, 255, 0)), 2)
	};
};

#endif