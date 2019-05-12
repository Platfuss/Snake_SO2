#include <curses.h>

#include "Snake.h"


int main()
{
	initscr();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);


	Snake snake;
	snake.start();
	printw("Gra skonczona. Za 3 sekundy okno zostanie zamkniete");
	refresh();
	std::this_thread::sleep_for(std::chrono::seconds(3));
	endwin();
	return 0;
}