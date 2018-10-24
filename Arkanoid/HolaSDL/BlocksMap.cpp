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
					blocks[i][j] = new Block(xPixCell, yPixCell, refPos, i, j, col, blockText);
				else
					blocks[i][j] = nullptr;
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

/*Vector2D BlocksMap::checkCollision(Ball ball) {
	if (ball.pos == anyBlockPos)
		return Vector2D(ball.dir.x, ball.dir.y);
	else
		return Vector2D(0, 0);	//Vector direccion no valido
}*/

void BlocksMap::render() {
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			if (blocks[i][j] != nullptr)
				blocks[i][j]->render();
}
