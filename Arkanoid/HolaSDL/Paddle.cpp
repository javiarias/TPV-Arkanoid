#include "Paddle.h"


void Paddle::render()
{
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	texture->render(destRect);
}

void Paddle::update()
{
	if (checkLimits())
		pos = pos + dir;
}

void Paddle::handleEvents(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {

		if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
			dir.setX(-0.1);

		else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
			dir.setX(0.1);

		else
			dir.setX(0);
	}
	else
		dir.setX(0);
}

bool Paddle::checkLimits()
{
	return (dir.getX() < 0 && pos.getX() > wallWidth) || (dir.getX() > 0 && pos.getX() < (windowWidth - wallWidth - width)) || (dir.getX() == 0);
}

bool Paddle::getCollisionVector(const SDL_Rect& ballRect, Vector2D& collVector)
{
	SDL_Rect paddle, result;
	paddle.x = pos.getX();
	paddle.y = pos.getY();
	paddle.h = height;
	paddle.w = width;

	if (SDL_IntersectRect(&paddle, &ballRect, &result)) {
		//Choca con la parte superior del paddle
		if (result.w > result.h)
			collVector = Vector2D(0, -1);
		//Choca con los laterales
		else {
			//Izquierda
			if(paddle.x < ballRect.x)
				collVector = Vector2D(1, 0);
			//Derecha
			else
				collVector = Vector2D(-1, 0);
		}			
		return true;
	}
	return false;
}