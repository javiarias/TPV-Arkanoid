#pragma once
#include "Vector2D.h"
#include "Texture.h"

class Wall
{
private:
	Vector2D pos;
	Texture* texture = nullptr;
	uint width = 0, height = 0;

public:
	Wall(double x, double y, uint w, uint h, Texture* _texture) : texture(_texture), width(w), height(h) { pos = Vector2D(x, y); };
	void render();
};

