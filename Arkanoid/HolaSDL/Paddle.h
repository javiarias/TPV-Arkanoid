#pragma once
#include "Vector2D.h"
#include "Texture.h"

const int PADDLE_VEL = 9;

class Paddle
{
private:
	Vector2D pos, dir;
	uint width = 0, height = 0;
	Texture* texture = nullptr;
	uint windowWidth = 0, wallWidth = 0;
	bool limitLeft, limitRight;

	bool checkLimits();

public:
	Paddle(uint x, uint y, uint w, uint h, uint _windowWidth, uint _wallWidth, Texture* _texture) : width(w), height(h), texture(_texture), windowWidth(_windowWidth), wallWidth(_wallWidth) { pos = Vector2D(x, y); dir = Vector2D(0, 0); };
	void render() const;
	void update();
	void handleEvents(SDL_Event event);
	bool getCollisionVector(const SDL_Rect& ballRect, Vector2D& collVector) const;
};

