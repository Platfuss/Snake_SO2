#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <curses.h>

class Snake
{
public:
	Snake();
	~Snake();
	void start();
private:
	std::mutex mtx;
	std::condition_variable condVarClearMap, 
							condVarAddFrame,
							condVarAddSnake,
							condVarAddFood,
							condVarTick;
	std::vector < std::vector <unsigned char> > map_;
	std::vector < std::pair <int, int> > snakeBody_;

	std::pair <int, int> foodPosition_{ -1, -1 };

	const int mapHorizontal_ = 30;
	const int mapVertical_ = 25;
	bool endGame_ = false;
	bool frameReady_ = false;
	bool snakeReady_ = false;
	bool nextTick_ = false;
	int direction_ = 0;

	void clearMap();
	void addFrame();
	void addSnake();
	void addFood();
	void draw();
	void tick();
	void changeDirection();
	void moveSnake();
};

