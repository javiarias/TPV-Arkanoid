#include "LivesCounter.h"



LivesCounter::LivesCounter(uint _numLives, uint xPos, uint yPos, uint _height, uint _width, Texture* _texture) {
	numLives = _numLives;
	pos = Vector2D(xPos, yPos);
	height = _height;
	width = _width;
	texture = _texture;
}


uint LivesCounter::getNumLives() {
	return numLives;
}

void LivesCounter::countDown() {
	numLives--;
}

void LivesCounter::setLivesTo(uint _lives) {
	numLives = _lives;
}

//Renderiza
void LivesCounter::render(uint offSet) {
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;

	for (uint i = 0; i < numLives; i++) {
		texture->render(destRect);
		destRect.x += offSet;
	}
}

