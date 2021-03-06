#pragma once
#include "Vector2D.h"
#include "Texture.h"
#include "checkML.h"

class Game;

class Ball {
private:
	Vector2D pos, vel;
	uint height = 0, width = 0;
	Texture* texture = nullptr;
	Game* game = nullptr;

	SDL_Rect getDestRect() const;

public:
	Ball(uint x, uint y, uint _height, uint _width, Texture* _texture, Game* _game);
	void render() const;
	void update();
};

