#include "Ball.h"
#include "Game.h"


Ball::Ball(uint x, uint y, uint _height, uint _width, Texture* _texture, Game* _game)
{
	pos = Vector2D(x, y);
	vel = Vector2D(0.7 * 1, -0.4 * 1);
	height = _height;
	width = _width;
	texture = _texture;
	game = _game;
}


void Ball::render() {
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	texture->render(destRect);
}

SDL_Rect Ball::getDestRect() {
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	return destRect;
}

void Ball::update() {
	Vector2D prevPos = pos;

	pos = pos + vel;

	Vector2D collVector;
	SDL_Rect ballRect = getDestRect();

	if (game->collides(ballRect, vel, collVector)) {
		vel = vel - (collVector * (vel * collVector  *2));
		pos = prevPos + vel;
	}
}


