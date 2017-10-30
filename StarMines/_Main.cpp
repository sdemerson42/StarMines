#include <conio.h>
#include <iostream>

#include "GameState.h"


int main()
{
	auto gs = new GameState;
	gs->exec();
	delete gs;

	_getch();
}