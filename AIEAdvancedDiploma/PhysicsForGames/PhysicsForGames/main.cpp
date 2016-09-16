#include "PhysicsForGamesApp.h"

int main() {
	
	auto app = new PhysicsForGamesApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}