#pragma once
#include "Vector2D.h"
#include "Ball.h"
#include "Block.h"
#include <iostream>
#include <fstream>

class BlocksMap
{

private:
	Block*** blocks = nullptr;
	uint xSize = 0, ySize = 0, xPixTotal = 0, yPixTotal = 0, xPixCell = 0, yPixCell = 0;
	Texture* blockText = nullptr;
	Vector2D* refPos;

public:
	BlocksMap(string file, uint refX, uint refY, uint WindowXSize, uint WindowYSize, Texture* texture);
	~BlocksMap();
	void load(string file);
	void render();
	int getBlockAmount();
	//Vector2D checkCollision(Ball ball);
};

