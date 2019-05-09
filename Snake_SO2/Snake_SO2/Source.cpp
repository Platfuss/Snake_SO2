#include <curses.h>

#include "Snake.h"



int main()
{
	initscr();
	Snake snake;
	snake.start();

	_getch();
	return 0;
}