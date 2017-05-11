#include <iostream>
#include "PhysicsApp.h"

int main()
{
	std::cout << "ADM_Tester..." << std::endl;

	PhysicsApp * app = new PhysicsApp();

	if (app != nullptr) {
		app->run("AIE", SCREEN_W, SCREEN_H, false);
	}

	delete app;

	std::cout << "...Finished!" << std::endl;
	return 0;
}