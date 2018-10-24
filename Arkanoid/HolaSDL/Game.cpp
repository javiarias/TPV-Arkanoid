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
	{
		textures[i] = new Texture(renderer, IMG_PATH + FILES[i].filename, FILES[i].rows, FILES[i].columns);
	}
	// create the game objects
	leftWall = new Wall(0, 0, WALL_WIDTH, WIN_HEIGHT, textures[SideTex]);
	rightWall = new Wall(WIN_WIDTH - WALL_WIDTH, 0, WALL_WIDTH, 600, textures[SideTex]);
	topWall = new Wall(0, 0, WIN_WIDTH, WALL_WIDTH, textures[TopTex]);
	ball = new Ball(395, 480, 10, 10, textures[BallTex], this);
	paddle = new Paddle(350, 500, 100, 10, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
	blocksMap = new BlocksMap(MAP_PATH + "level01.ark", WALL_WIDTH, WALL_WIDTH, WIN_WIDTH, WIN_HEIGHT, textures[BlockTex]);
}

Game::~Game() {
	for (uint i = 0; i < NUM_TEXTURES; i++) delete textures[i];
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run() {
	while (!exit) { // Falta el control de tiempo
		handleEvents();
		update();
		render();
	}
}

void Game::update() {
	paddle->update();
	ball->update();
}

void Game::render() const {
	SDL_RenderClear(renderer);

	ball->render();
	//blocksMap->render();
	paddle->render();
	leftWall->render();
	rightWall->render();
	topWall->render();

	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT) exit = true;
		paddle->handleEvents(event);
	}
}


Vector2D Game::collision(Vector2D pos, uint width, uint height)
{
	//check walls

	//left
	if (pos.getX() <= WALL_WIDTH)
		return Vector2D(1, 0);
	//right

	//top






	//bottom (WIP)

	else
		return pos;
}