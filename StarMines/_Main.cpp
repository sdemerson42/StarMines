#include <conio.h>
#include <iostream>

#include "GameState.h"
#include <time.h>

int main()
{
	//DEBUG
	srand(time(0));
	auto gs = new GameState;
	gs->exec();
	delete gs;
	std::cout << "GameState deleted. Session end.\n";

	_getch();
}