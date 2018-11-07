#pragma once
#include "Vector2D.h"
#include "Ball.h"
#include "Block.h"
#include <iostream>
#include <fstream>
#include "checkML.h"

class BlocksMap {
private:
	Block*** blocks = nullptr;
	uint xSize = 0, ySize = 0, xPixTotal = 0, yPixTotal = 0, xPixCell = 0, yPixCell = 0;
	Texture* blockText = nullptr;
	Vector2D* refPos;

public:
	BlocksMap(string file, uint refX, uint refY, uint WindowXSize, uint WindowYSize, Texture* texture);
	~BlocksMap();
	void load(string file);
	void render() const;
	int getBlockAmount() const;
	Block* collides(const SDL_Rect& ballRect, const Vector2D& ballVel, Vector2D& collVector) const;
	Block* blockAt(const Vector2D& p) const;
	void destroy(Block* block);
};

