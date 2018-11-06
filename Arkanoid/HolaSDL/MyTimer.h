#pragma once

#include "Texture.h"
#include "Vector2D.h"

#include <string>

class MyTimer
{
private:
	Texture* digits = nullptr;
	Vector2D pos;
	uint height = 0, width = 0;
	int secs = 0, mins = 0;
public:
	MyTimer(Texture* _texture, uint x, uint y, uint _height, uint _width);

	void update(uint ms);

	void render();
};

