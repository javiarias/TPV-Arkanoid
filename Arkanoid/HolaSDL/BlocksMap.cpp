#include "BlocksMap.h"

BlocksMap::BlocksMap(string file, uint refX, uint refY, uint WindowXSize, uint WindowYSize, Texture* texture)
{
	xPixTotal = WindowXSize - 2 * refX;
	yPixTotal = WindowYSize/2;
	blockText = texture;
	refPos = new Vector2D(refX, refY);
	load(file);
}

BlocksMap::~BlocksMap()
{
	for (int i = 0; i < xSize; i++)
	{
		for (int j = 0; j < ySize; j++)
			delete blocks[i][j];
		delete[] blocks[i];
	}
	delete[] blocks;
	delete blockText;
	delete refPos;
}

void BlocksMap::load(string file) {
	ifstream input;
	input.open(file);

	if (input.is_open()) {
		int x, y;
		input >> x >> y;

		xSize = x;
		ySize = y;
		blocks = new Block**[xSize];
		for (int i = 0; i < x; i++)
			blocks[i] = new Block*[ySize];

		xPixCell = xPixTotal / xSize;
		yPixCell = yPixTotal / ySize;


		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
			{
				int col;
				input >> col;
				if (col != 0)
					blocks[j][i] = new Block(xPixCell, yPixCell, refPos, i, j, col, blockText);
				else
					blocks[j][i] = nullptr;
			}
		input.close();
	}
}

int BlocksMap::getBlockAmount() {
	int ret = 0;

	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			if (blocks[i][j] != nullptr)
				ret++;

	return ret;
}

Block* BlocksMap::blockAt(const Vector2D& p) {
	uint x = (p.getX() - refPos->getX()) / xPixCell;
	uint y = (p.getY() - refPos->getY()) / yPixCell;
	return blocks[x][y];
}

Block* BlocksMap::collides(const SDL_Rect& ballRect, const Vector2D& ballVel, Vector2D& collVector) {

	Block* b = nullptr;
	if (!(ballRect.y >= yPixTotal || ballRect.x >= xPixTotal || ballRect.y < 0 || ballRect.x < 0)) {
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

void BlocksMap::render() {
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++) {
			if (blocks[i][j] != nullptr)
				blocks[i][j]->render();
		}
}
