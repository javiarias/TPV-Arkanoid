#pragma once
#include "Texture.h"
#include "Vector2D.h"
#include "checkML.h"

class LivesCounter
{
private:
	uint numLives;
	Vector2D pos;
	uint height = 0, width = 0;
	Texture* texture = nullptr;
public:
	LivesCounter(uint _numLives, uint xPos, uint yPos, uint _height , uint _width, Texture* _texture);

	uint getNumLives();
	void countDown();
	void setLivesTo(uint _lives);

	void render();
};

