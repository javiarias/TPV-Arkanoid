#include "Game.h"
#include "checkML.h"

using namespace std;
using uint = unsigned int;

void GameLoop() {
	Game game = Game();
	game.run();
}

int main(int argc, char* argv[]){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks

	try {
		GameLoop();
	}
	catch (string ex) {
		cout << ex << endl;
		//Linea que pausa el programa para ver si escribe la excepcion en consola
		system("pause");
	}
	return 0;
}