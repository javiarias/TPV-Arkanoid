#include "Paddle.h"

//Renderiza
void Paddle::render() const {
	SDL_Rect destRect;
	destRect.x = pos.getX();
	destRect.y = pos.getY();
	destRect.h = height;
	destRect.w = width;
	texture->render(destRect);
}

void Paddle::update() {
	if (checkLimits())
		pos = pos + dir;
}

//Input del jugador
void Paddle::handleEvents(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
			dir.setX(-PADDLE_VEL);
		else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
			dir.setX(PADDLE_VEL);
		else
			dir.setX(0);
	}
	else
		dir.setX(0);
}

bool Paddle::checkLimits() {
	return (dir.getX() < 0 && pos.getX() > wallWidth) || (dir.getX() > 0 && pos.getX() < (windowWidth - wallWidth - width)) || (dir.getX() == 0);
}

bool Paddle::getCollisionVector(const SDL_Rect& ballRect, Vector2D& collVector) const {
	SDL_Rect paddle, result;
	paddle.x = pos.getX();
	paddle.y = pos.getY();
	paddle.h = height;
	paddle.w = width;

	if (!(ballRect.x + ballRect.w > paddle.x && ballRect.y >= paddle.y && ballRect.x < paddle.x + paddle.w))
		if (SDL_IntersectRect(&paddle, &ballRect, &result)) {
			//Choca con la parte superior del paddle
			if (result.w > result.h) {
				//Dependiendo del punto de incidencia, devuelve el vector normal
				//Si choca en la seccion media de la pala(segmento que equivale a dos bolas), vector de 90 grados
				if(ballRect.x > paddle.x + paddle.w/2 - ballRect.w && ballRect.x < paddle.x + paddle.w / 2)
					collVector = Vector2D(0, -1);
				else {	
					//Calculamos el vector normal, haciendo un vector de A(parte baja de la pala) a B(esquina de la bola). 
					//Nota: a la Y se le resta un offset para evitar que la bola rebote el horizontal
					int offSet = 275;	//Para menos rango de angulo, mayor offSet
					//Izquierda
					if (ballRect.x <= paddle.x + paddle.w / 2 - ballRect.w) {
						collVector = Vector2D(ballRect.x - (paddle.x + paddle.w / 2 - ballRect.w), ballRect.y - (paddle.y + paddle.h) - offSet);
						collVector.normalize();
					}
					//Derecha
					else if (ballRect.x >= paddle.x + paddle.w / 2) {
						collVector = Vector2D((ballRect.x + ballRect.w) - (paddle.x + paddle.w / 2 + ballRect.w), ballRect.y - (paddle.y + paddle.h) - offSet);
						collVector.normalize();
					}
					else //Se devuleve por si acaso
						collVector = Vector2D(0, -1);
				}
			}
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