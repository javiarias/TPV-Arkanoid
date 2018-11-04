#pragma once
#include "Vector2D.h"
#include "Texture.h"

class Block
{
private:
	Vector2D* referencePoint;
	uint width = 0, height = 0, row = 0, column = 0, color = 0;
	Texture* texture = nullptr;

public:
	Block(uint w, uint h, Vector2D* refPos, uint _row, uint _column, uint _color, Texture* _texture) : width(w), height(h), column(_column), row(_row), color(_color), texture(_texture) { referencePoint = refPos; };
	void render() const;
	uint getY() const { return ((row * height) + referencePoint->getY()); };
	uint getX() const { return ((column * width) + referencePoint->getX()); };
	uint getW() const { return width; };
	uint getH() const { return height; };
};

