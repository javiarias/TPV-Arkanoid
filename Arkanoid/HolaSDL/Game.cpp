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
	leftWall = new Wall(0, TOP_MARGIN, WALL_WIDTH, WIN_HEIGHT - TOP_MARGIN, textures[SideTex]);
	rightWall = new Wall(WIN_WIDTH - WALL_WIDTH, TOP_MARGIN + WALL_WIDTH, WALL_WIDTH, WIN_HEIGHT - TOP_MARGIN, textures[SideTex]);
	topWall = new Wall(0, TOP_MARGIN, WIN_WIDTH, WALL_WIDTH, textures[TopTex]);
	ball = new Ball((WIN_WIDTH / 2) - BALL_SIZE / 2, WIN_HEIGHT - (WIN_HEIGHT / 10) - 2 * BALL_SIZE, BALL_SIZE, BALL_SIZE, textures[BallTex], this, false);
	paddle = new Paddle((WIN_WIDTH / 2) - PADDLE_WIDTH / 2, WIN_HEIGHT - (WIN_HEIGHT / 10), PADDLE_WIDTH, BALL_SIZE, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
	blocksMap = new BlocksMap(MAP_PATH + mapFiles[currentLevel], WALL_WIDTH, WALL_WIDTH + TOP_MARGIN, WIN_WIDTH, WIN_HEIGHT, textures[BlockTex]);
	// Timer
	timer = new MyTimer(textures[DigitsTex],(WIN_WIDTH - 15 * 5) / 2, (TOP_MARGIN - 15) / 2, 15, 15);
	// Vidas
	livesCounter = new LivesCounter(MAX_LIVES, TOP_MARGIN / 2 + (BALL_SIZE + TOP_MARGIN / 2), TOP_MARGIN / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE, textures[BallTex]);
	/*for (uint i = 0; i < MAX_LIVES; i++)
		livesTextures[i] = new Ball(TOP_MARGIN / 2 + (BALL_SIZE + TOP_MARGIN / 2) * i, TOP_MARGIN / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE, textures[BallTex], this, false);*/

	//lives = MAX_LIVES;
}

Game::~Game() {
	cleanGame();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::cleanGame() {
	for (uint i = 0; i < NUM_TEXTURES; i++) delete textures[i];
	for (uint i = 0; i < MAX_LIVES; i++) delete livesTextures[i];
	delete leftWall;
	delete rightWall;
	delete topWall;
	delete ball;
	delete paddle;
	delete blocksMap;
}

void Game::run() {
	render();
	ballPause();

	while (!exit) {
		if (!gameOver) { //si el juego no se ha acabado de alguna forma (ganado o perdido)
			uint frameStart = SDL_GetTicks();

			//Si el nivel ha acabado, carga el siguiente
			if (endLevel) {
				loadNextLevel();
				if (!gameOver) {
					render();
					dead = true;
					ballPause();
				}
			}
			else {
				handleEvents();
				update();
				render();
				//Si la bola "muere"
				if (dead) {
					livesCounter->countDown();
					if (livesCounter->getNumLives() > 0) {
						delete ball;
						ball = new Ball((WIN_WIDTH / 2) - BALL_SIZE / 2, WIN_HEIGHT - (WIN_HEIGHT / 10) - 2 * BALL_SIZE, BALL_SIZE, BALL_SIZE, textures[BallTex], this, false);
						delete paddle;
						paddle = new Paddle((WIN_WIDTH / 2) - PADDLE_WIDTH / 2, WIN_HEIGHT - (WIN_HEIGHT / 10), PADDLE_WIDTH, BALL_SIZE, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
						render();
						ballPause();
					}
					else
						gameOver = true;
				}
			}
				uint frameDuration = SDL_GetTicks() - frameStart;
				if (frameDuration < FRAME_CONTROL)
					SDL_Delay(FRAME_CONTROL - frameDuration);
				time += FRAME_CONTROL;
				timer->update(time);
		}

		else {
			GameOver();
		}
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
	//Timer
	timer->render();
	//Vidas
	livesCounter->render(20);

	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (dead) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				dead = false;
			else if (event.type == SDL_QUIT)
				exit = true;
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP_ENTER) //DEBUG
				endLevel = true;
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP_ENTER) //DEBUG
			endLevel = true;
		else {
			if (event.type == SDL_QUIT)
				exit = true;
			paddle->handleEvents(event);
		}
	}
}

bool Game::collides(const SDL_Rect& rect, const Vector2D& vel, Vector2D& collVector) {
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
			//Si no quedan bloques carga el nivel siguiente
			if (blocksMap->getBlockAmount() == 0)
				endLevel = true;
			return true;
		}
		return false;
	}
}

//Opcional
void Game::loadNextLevel() {

	//Subimos de nivel
	currentLevel++;
	//Si quedan niveles carga el siguiente, si no, exit = true, sale del juego
	if (currentLevel < NUM_LEVELS) {
		delete ball;
		delete paddle;
		delete blocksMap;

		ball = new Ball((WIN_WIDTH / 2) - BALL_SIZE / 2, WIN_HEIGHT - (WIN_HEIGHT / 10) - 2 * BALL_SIZE, BALL_SIZE, BALL_SIZE, textures[BallTex], this, false);
		paddle = new Paddle((WIN_WIDTH / 2) - PADDLE_WIDTH / 2, WIN_HEIGHT - (WIN_HEIGHT / 10), PADDLE_WIDTH, BALL_SIZE, WIN_WIDTH, WALL_WIDTH, textures[PaddleTex]);
		blocksMap = new BlocksMap(MAP_PATH + mapFiles[currentLevel], WALL_WIDTH, WALL_WIDTH + TOP_MARGIN, WIN_WIDTH, WIN_HEIGHT, textures[BlockTex]);
		endLevel = false;

		livesCounter->setLivesTo(MAX_LIVES);
	}
	else
		gameOver = true;
}

void Game::GameOver() {

	exit = true;
	SDL_DestroyWindow(window);

	if (dead) 
		cout << "GAME OVER" << endl;
	else {
		cout << "Juego acabado!" << endl;
	}
	scoreboard();
	system("pause");
}

void Game::ballPause() {
	while (dead && !exit && !endLevel)
		handleEvents();
}

void Game::scoreboard() {
	uint count = 0;
	if(!dead)
		writeScoreboard(count);

	cout << endl << "-----Scoreboard-----" << endl << endl;
	ifstream input;
	input.open(SAVE_PATH + "scoreboard.ark");
	for (int i = 1; i <= 10; i++) {
		cout << i << ": ";

		if (input.eof())
			cout << "--:--";
		else {
			uint aux;
			input >> aux;
			if ((aux / 1000) / 60 < 10)
				cout << "0";
			
			cout << (aux / 1000) / 60;

			cout << ":";

			if ((aux / 1000) % 60 < 10)
				cout << "0";

			cout << (aux / 1000) % 60;

			if (count == i)
				cout << " !!!";
		}

		cout << endl;
	}

	cout << endl;
	input.close();
}

void Game::writeScoreboard(uint& c) {
	ifstream input;
	input.open(SAVE_PATH + "scoreboard.ark");

	if (!input.is_open()) {
		ofstream output;
		output.open(SAVE_PATH + "scoreboard.ark");
		output << time << endl;

		output.close();
	}
	else {
		uint timeCompare = 0, count = 0;
		for (int i = 1; i <= 10 && timeCompare < time && !input.eof(); i++) {
			input >> timeCompare;
			if (timeCompare > time || input.eof())
				count = i;
		}
		if (count != 0) {
			c = count;
			string temp;
			input.clear();
			input.seekg(0, std::ios::beg);
			for (int i = 1; i <= 9 && !input.eof(); i++) {
				if (i == count) {
					temp = temp + to_string(time);
					temp = temp + " ";
				}
				string aux;
				input >> aux;
				temp = temp + aux;
				temp = temp + " ";
			}

			ofstream output;
			output.open(SAVE_PATH + "scoreboard.ark");
			output << temp;

			output.close();
		}
	}
	input.close();
}