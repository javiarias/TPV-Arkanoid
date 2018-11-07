#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Texture.h"
#include "BlocksMap.h"
#include "Paddle.h"
#include "Wall.h"
#include <string>
#include "Ball.h"
#include "MyTimer.h"
#include "LivesCounter.h"
#include "checkML.h"

//Para cambiar entre DEBUG y RELEASE
const bool DEBUG = true;
//Dimensiones de Pantalla
const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;
//Constantes
const uint NUM_TEXTURES = 6;
const uint NUM_LEVELS = 3;

const enum TextureName {
	BallTex,
	BlockTex,
	PaddleTex,
	SideTex,
	TopTex,
	DigitsTex
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
	{"topside.png", 1, 1},
	{"digits.jpeg", 3, 4}
};
const string mapFiles[NUM_LEVELS] = { "level01.ark","level02.ark" ,"level03.ark" };

const string IMG_PATH = "..\\images\\";
const string MAP_PATH = "..\\maps\\";
const string FONT_PATH = "..\\fonts\\";
const string SAVE_PATH = "..\\";
const uint WALL_WIDTH = 10;
const uint FRAMERATE = 60;
const uint FRAME_CONTROL = (1000 / FRAMERATE);
const uint MAX_LIVES = 3;
const uint TOP_MARGIN = 20;
const uint BALL_SIZE = 10;
const uint PADDLE_WIDTH = 100;

class Game
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool exit = false;
	bool dead = true;
	bool gameOver = false;
	bool endLevel = false;
	Texture* textures[NUM_TEXTURES];
	Ball* ball = nullptr;
	Paddle* paddle = nullptr;
	BlocksMap* blocksMap = nullptr;
	Wall* leftWall = nullptr;
	Wall* rightWall = nullptr;
	Wall* topWall = nullptr;
	MyTimer* timer = nullptr;
	LivesCounter* livesCounter = nullptr;
	void cleanGame();

	uint currentLevel = 0;

	uint time = 0;
	
	void loadNextLevel();
	//Auxiliar
	void GameOver();
	void render() const;
	void handleEvents();
	void update();
	void ballPause();
	void scoreboard();
	void writeScoreboard(uint& c);

public:
	Game();
	~Game();
	void run();
	bool collides(const SDL_Rect& rect, const Vector2D& vel, Vector2D& collVector);
};

