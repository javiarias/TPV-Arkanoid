#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#include "BlocksMap.h"
#include "Paddle.h"
#include "Wall.h"
#include <string>
#include "Ball.h"

const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;
const uint NUM_TEXTURES = 5;
const enum TextureName {
	BallTex,
	BlockTex,
	PaddleTex,
	SideTex,
	TopTex
};
const struct File {
	string filename;
	uint rows, columns;
};
const File FILES[NUM_TEXTURES] =
{
	{"ball.png", 1, 1},
	{"bricks.png", 2, 3},
	{"paddle.png", 1, 1},
	{"side.png", 1, 1},
	{"topside.png", 1, 1}
};
const string IMG_PATH = "..\\images\\";
const string MAP_PATH = "..\\maps\\";
const uint WALL_WIDTH = 10;

class Game
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool exit = false;
	Texture* textures[NUM_TEXTURES];
	Ball* ball = nullptr;
	Paddle* paddle = nullptr;
	BlocksMap* blocksMap = nullptr;
	Wall* leftWall = nullptr;
	Wall* rightWall = nullptr;
	Wall* topWall = nullptr;

public:
	Game();
	~Game();
	void run();
	void render() const;
	void handleEvents();
	void update();
	Vector2D collision(Vector2D pos, uint width, uint height);
};

