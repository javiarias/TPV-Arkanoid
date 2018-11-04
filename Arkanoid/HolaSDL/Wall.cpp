#include "Wall.h"



void Wall::render() const
{
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	texture->render(destRect);
}

bool Wall::collides(const SDL_Rect& rect, Vector2D& collVector) const {
	SDL_Rect wall;
	wall.x = pos.getX();
	wall.y = pos.getY();
	wall.h = height;
	wall.w = width;

	if (SDL_HasIntersection(&wall, &rect)) {
		if (width > height)
			collVector = Vector2D(0, 1);
		else if(wall.x = 0)
			collVector = Vector2D(1, 0);
		else
			collVector = Vector2D(-1, 0);
		return true;
	}
	return false;
}
