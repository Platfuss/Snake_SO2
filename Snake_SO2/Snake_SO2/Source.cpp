#include <curses.h>

#include "Snake.h"


int main()
{
	initscr();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);


	Snake snake;
	snake.start();

	printw("Gra skonczona");

	getch();
	endwin();
	return 0;
}