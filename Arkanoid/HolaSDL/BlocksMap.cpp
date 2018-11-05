#include "BlocksMap.h"
#include <cmath>

BlocksMap::BlocksMap(string file, uint refX, uint refY, uint WindowXSize, uint WindowYSize, Texture* texture)
{
	xPixTotal = WindowXSize - 2 * refX;
	yPixTotal = WindowYSize/2;
	blockText = texture;
	refPos = new Vector2D(refX, refY);
	load(file);
}

BlocksMap::~BlocksMap() {
	for (int i = 0; i < xSize; i++)	{
		for (int j = 0; j < ySize; j++) {
			delete blocks[i][j];
			blocks[i][j] = nullptr;
		}
		delete[] blocks[i];
		blocks[i] = nullptr;
	}
	delete[] blocks;
	blocks = nullptr;
	delete refPos;
	refPos = nullptr;
}

void BlocksMap::load(string file) {
	ifstream input;
	input.open(file);

	if (input.is_open()) {
		int x, y;
		input >> y >> x;

		xSize = x;
		ySize = y;
		blocks = new Block**[xSize];
		for (int i = 0; i < x; i++)
			blocks[i] = new Block*[ySize];

		xPixCell = xPixTotal / xSize;
		yPixCell = yPixTotal / ySize;

		for (int j = 0; j < y; j++)
			for (int i = 0; i < x; i++)	{
				int col;
				input >> col;
				if (col != 0)
					blocks[i][j] = new Block(xPixCell, yPixCell, refPos, j, i, col, blockText);
				else
					blocks[i][j] = nullptr;
			}
		input.close();
	}
	else throw "Level file not found. File path should be: " + file;
}

int BlocksMap::getBlockAmount() const {
	int ret = 0;

	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			if (blocks[i][j] != nullptr)
				ret++;

	return ret;
}

Block* BlocksMap::blockAt(const Vector2D& p) const {
	uint x = trunc((double)((p.getX() - refPos->getX()) / xPixCell)); // el programa nos daba errores a la hora de calcular la columna y la fila, 
																	  // y nos comentaron que dejáramos esto por si acaso
	uint y = trunc((double)((p.getY() - refPos->getY()) / yPixCell));
	if (x >= 0 && x < xSize && y >= 0 && y < ySize)
		return blocks[x][y];
	else
		return nullptr;
}

Block* BlocksMap::collides(const SDL_Rect& ballRect, const Vector2D& ballVel, Vector2D& collVector) const {

	Block* b = nullptr;
	if (ballRect.y < (yPixTotal + refPos->getY()) && ballRect.x < xPixTotal && ballRect.y >= 0 && ballRect.x >= 0) { 
		Vector2D p0 = { (double)ballRect.x, (double)ballRect.y }; // top-left
		Vector2D p1 = { (double)(ballRect.x + ballRect.w), (double)ballRect.y }; // top-right
		Vector2D p2 = { (double)ballRect.x, (double)(ballRect.y + ballRect.h) }; // bottom-left
		Vector2D p3 = { (double)(ballRect.x + ballRect.w), (double)(ballRect.y + ballRect.h) }; // bottom-right
		if (ballVel.getX() < 0 && ballVel.getY() < 0) {
			if ((b = blockAt(p0))) {
				if (b != nullptr) {
					if ((b->getY() + b->getH() - p0.getY()) <= (b->getX() + b->getW() - p0.getX()))
						collVector = { 0,1 }; // Borde inferior mas cerca de p0
					else
						collVector = { 1,0 }; // Borde dcho mas cerca de p0
				}
			}
			else if ((b = blockAt(p1)))
				if (b != nullptr)
					collVector = { 0,1 };
			else if ((b = blockAt(p2)))
					if (b != nullptr)
						collVector = { 1,0 };
		}
		else if (ballVel.getX() >= 0 && ballVel.getY() < 0) {
			if ((b = blockAt(p1))) {
				if (b != nullptr) {
					if (((b->getY() + b->getH() - p1.getY()) <= (p1.getX() - b->getX())) || ballVel.getX() == 0)
						collVector = { 0, 1 }; // Borde inferior mas cerca de p1
					else
						collVector = { -1,0 }; // Borde izqdo mas cerca de p1
				}
			}
			else if ((b = blockAt(p0)))
				if (b != nullptr)
					collVector = { 0,1 };
			else if ((b = blockAt(p3)))
				if (b != nullptr)
					collVector = { -1,0 };
		}
		else if (ballVel.getX() > 0 && ballVel.getY() > 0) {
			if ((b = blockAt(p3))) {
				if (b != nullptr) {
					if (((p3.getY() - b->getY()) <= (p3.getX() - b->getX()))) // || ballVel.getX() == 0)
						collVector = { 0,-1 }; // Borde superior mas cerca de p3
					else
						collVector = { -1,0 }; // Borde dcho mas cerca de p3
				}
			}
			else if ((b = blockAt(p2)))
				if (b != nullptr)
					collVector = { 0,-1 };
			else if ((b = blockAt(p1)))
				if (b != nullptr)
					collVector = { -1,0 };
		}
		else if (ballVel.getX() < 0 && ballVel.getY() > 0) {
			if ((b = blockAt(p2))) {
				if (b != nullptr) {
					if (((p2.getY() - b->getY()) <= (b->getX() + b->getW() - p2.getX()))) // || ballVel.getX() == 0)
						collVector = { 0,-1 }; // Borde superior mas cerca de p2
					else
						collVector = { 1,0 }; // Borde dcho mas cerca de p0
				}
			}
			else if ((b = blockAt(p3)))
				if (b != nullptr)
					collVector = { 0,-1 };
			else if ((b = blockAt(p0)))
					if (b != nullptr)
						collVector = { 1,0 };
		}
	}
	return b;
}

void BlocksMap::destroy(Block* block) {
	uint x = (block->getX() - refPos->getX()) / xPixCell;
	uint y = (block->getY() - refPos->getY()) / yPixCell;
	delete blocks[x][y];
	blocks[x][y] = nullptr;
}

void BlocksMap::render() const {
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++) {
			if (blocks[i][j] != nullptr)
				blocks[i][j]->render();
		}
}
