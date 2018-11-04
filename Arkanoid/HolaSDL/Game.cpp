#include "Game.h"


Game::Game() {
	// initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr) throw "Error loading the SDL window or renderer";
	// create the textures
	for (uint i = 0; i < NUM_TEXTURES; i++)
		textures[i] = new Texture(renderer, IMG_PATH + FILES[i].filename, FILES[i].rows, FILES[i].columns);

	// create the game objects
	leftWall = new Wall(0, 0, WALL_WIDTH, WIN_HEIGHT, textures[SideTex]);
	rightWall = new Wall(WIN_WIDTH - WALL_WIDTH, 0, WALL_WIDTH, 600, textures[SideTex]);
	topWall = new Wall(0, 0, WIN_WIDTH, WALL_WIDTH, textures[TopTex]);
	ball = new Ball(WIN_WIDTH/2 - 10, WIN_HEIGHT - 100 - 15, 10, 10, textures[BallTex], this);
	paddle = new Paddle(WIN_WIDTH/2 - 50, WIN_HEIGHT - 100, 100, 10, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
	blocksMap = new BlocksMap(MAP_PATH + "levsel01.ark", WALL_WIDTH, WALL_WIDTH, WIN_WIDTH, WIN_HEIGHT, textures[BlockTex]);
}

Game::~Game() {
	for (uint i = 0; i < NUM_TEXTURES; i++) delete textures[i];
	delete leftWall;
	delete rightWall;
	delete topWall;
	delete ball;
	delete paddle;
	delete blocksMap;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run() {
	while (!exit) {
		uint frameStart = SDL_GetTicks();

		handleEvents();
		update();
		render();
		if (dead) {
			delete ball;
			ball = new Ball(395, 480, 10, 10, textures[BallTex], this);
			delete paddle;
			paddle = new Paddle(350, 500, 100, 10, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
			render();
			while (dead)
				handleEvents();
		}

		uint frameDuration = SDL_GetTicks() - frameStart;
		if (frameDuration < FRAME_CONTROL)
			SDL_Delay(FRAME_CONTROL - frameDuration);
	}
}

void Game::update() {
	paddle->update();
	ball->update();
}

void Game::render() const {
	SDL_RenderClear(renderer);

	ball->render();
	blocksMap->render();
	paddle->render();
	leftWall->render();
	rightWall->render();
	topWall->render();

	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (dead) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				dead = false;
		}
		else {
			if (event.type == SDL_QUIT)
				exit = true;
			paddle->handleEvents(event);
		}
	}
}


bool Game::collides(const SDL_Rect& rect, const Vector2D& vel, Vector2D& collVector)
{
	//Collides con el muro izquierdo
	if (leftWall->collides(rect, collVector))
		return true;
	//Collides con el muro derecho
	else if (rightWall->collides(rect, collVector))
		return true;
	//Collides con el muro superior
	else if (topWall->collides(rect, collVector))
		return true;
	//Collides con el suelo (temporal)
	else if (rect.h + rect.y > WIN_HEIGHT - WALL_WIDTH) {
		dead = true;
		return false;
	}
	//Collides con el paddle
	else if (paddle->getCollisionVector(rect, collVector)) { 
		return true;
	}
	//Collides con algun bloque 
	else {
		Block* block = blocksMap->collides(rect, vel, collVector);
		if (block != nullptr) {
			blocksMap->destroy(block);
			/*if (blocksMap->getBlockAmount() == 0)
				win = true;*/
			return true;
		}

		return false;
	}
}