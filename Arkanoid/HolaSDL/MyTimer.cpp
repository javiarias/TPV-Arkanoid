#include "MyTimer.h"



MyTimer::MyTimer(Texture* _texture, uint x, uint y, uint _height, uint _width)
{
	digits = _texture;
	height = _height;
	width = _width;
	pos = Vector2D(x, y);
}


void MyTimer::render() {
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;

	//Dos primeros digitos (minutos)
	string minutes = to_string(mins);
	//Si hay mas de una decena de minutos
	if (minutes.size() > 1) {
		//Primer digito
		int a = minutes[minutes.size() - 2] - '0';
		int b = minutes[minutes.size() - 1] - '0';
		digits->renderFrame(destRect, a / 4, a % 4);
		destRect.x += 15;
		//Segundo digito
		digits->renderFrame(destRect, b / 4, b % 4);
	}
	else {
		//Primer digito
		digits->renderFrame(destRect, 0, 0);
		destRect.x += 15;
		//Segundo digito
		int b = minutes[minutes.size() - 1] - '0';
		digits->renderFrame(destRect, b / 4, b % 4);
	}

	//Punto que separa minutos de segundos
	destRect.x += 15;
	digits->renderFrame(destRect, 2, 2);

	//Dos primeros digitos (segundos)
	string seconds = to_string(secs);

	destRect.x += 15;
	if (seconds.size() > 1) {
		//Primer digito
		int a = seconds[seconds.size() - 2] - '0';
		int b = seconds[seconds.size() - 1] - '0';
		digits->renderFrame(destRect, a / 4, a % 4);
		destRect.x += 15;
		//Segundo digito
		digits->renderFrame(destRect, b / 4, b % 4);
	}
	else {
		//Primer digito
		digits->renderFrame(destRect, 0, 0);
		destRect.x += 15;
		//Segundo digito
		int b = seconds[seconds.size() - 1] - '0';
		digits->renderFrame(destRect, b / 4, b % 4);
	}
}

void MyTimer::update(uint ms) {
	int secsAux = ms / 1000;
	secs = secsAux % 60;
	mins = secsAux / 60;
}
