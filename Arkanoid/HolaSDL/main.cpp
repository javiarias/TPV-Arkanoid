#include "Game.h"
#include "checkML.h"

using namespace std;

using uint = unsigned int;

void GameLoop() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks

	Game game = Game();

	game.run();
}

int main(int argc, char* argv[]){
	GameLoop();
	return 0;
}