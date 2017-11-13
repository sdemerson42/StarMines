#include <conio.h>
#include <iostream>

#include "GameState.h"
#include <time.h>

int main()
{
	srand(time(0));
	auto gs = new GameState;
	gs->exec();
	delete gs;

	_getch();
}