#include "Block.h"

Block::~Block()
{
	delete texture;
	delete referencePoint;
}

void Block::render() 
{
	SDL_Rect destRect;
	destRect.x = width * column + referencePoint->getX();
	destRect.y = height * row + referencePoint->getY();
	destRect.h = height;
	destRect.w = width;
	int row, col;
	row = (color > 3);
	col = (color % 3 + 3 * (color % 3 == 0)) - 1;

	texture->renderFrame(destRect, row, col);
}
