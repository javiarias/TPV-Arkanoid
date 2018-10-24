#include "Wall.h"



void Wall::render()
{
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	texture->render(destRect);
}
