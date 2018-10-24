#include "Ball.h"
#include "Game.h"


Ball::Ball(uint x, uint y, uint _height, uint _width, Texture* _texture, Game* _game)
{
	pos = Vector2D(x, y);
	dir = Vector2D(-5, -1);
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

void Ball::changeDir(Vector2D collisionVector) {
	/*//Si colisiona con algo horizontal
	if (collisionVector.getX() > 0 || collisionVector.getX() < 0)
		dir.setY(-dir.getY());
	//Si no es horizontal, es vertical
	else
		dir.setX(-dir.getX());*/
	dir.setX(-dir.getX());
}

void Ball::update() {
	Vector2D prevPos = pos;

	pos = pos + dir;

	Vector2D collVector;
	SDL_Rect ballRect = getDestRect();

	if (game->collides(ballRect, vel, collVector)) {
		dir = dir - collVector * (2 * dir*collVector);
		pos = prevPos + dir;
	}
}


