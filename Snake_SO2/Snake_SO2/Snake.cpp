#include "Snake.h"



Snake::Snake()
{
	for (int i = 0; i < mapVertical_; ++i) {
		map_.push_back(std::vector<unsigned char>(mapHorizontal_));
	}
	std::pair <int, int> part (mapVertical_ / 2, mapHorizontal_ / 2);
	snakeBody_.push_back(part);
}



Snake::~Snake()
{
}



void Snake::start()
{
	srand(static_cast<unsigned int> (time(NULL)));

	std::thread clearMapThread (&Snake::clearMap, this);

	std::thread addFrameThread (&Snake::addFrame, this);
	while (!frameReady_)
		std::this_thread::yield;

	std::thread addSnakeThread (&Snake::addSnake, this);
	while (!snakeReady_)
		std::this_thread::yield;

	std::thread addFoodThread (&Snake::addFood, this);

	draw();

	std::thread changeDirectionThread (&Snake::tick, this);
	std::thread keyboard(&Snake::changeDirection, this);

	clearMapThread.join();
	addFrameThread.join();
	addSnakeThread.join();
	addFoodThread.join();
	changeDirectionThread.join();
	keyboard.join();
}



void Snake::clearMap()
{
	while (!endGame_) {
		std::unique_lock<std::mutex> lck(mtx);
		for (int i = 0; i < mapVertical_; ++i) {
			for (int j = 0; j < mapHorizontal_; ++j) {
				map_[i][j] = ' ';
			}
		}
		frameReady_ = snakeReady_ = nextTick_ = false;

		system("cls");
		condVarTick.notify_one();
		condVarClearMap.wait(lck, [this] {return nextTick_ == true; });
	}
}



void Snake::addFrame()
{
	while (!endGame_) {
		std::unique_lock<std::mutex> lck(mtx);
		for (int i = 1; i < mapVertical_ - 1; ++i) {
			map_[i][0] = map_[i][mapHorizontal_ - 1] = static_cast <unsigned char> (186);
		}

		for (int j = 1; j < mapHorizontal_ - 1; ++j) {
			map_[0][j] = map_[mapVertical_ - 1][j] = static_cast <unsigned char> (205);
		}

		map_[0][0] = static_cast <unsigned char> (201);
		map_[0][mapHorizontal_ - 1] = static_cast <unsigned char> (187);
		map_[mapVertical_ - 1][mapHorizontal_ - 1] = static_cast <unsigned char> (188);
		map_[mapVertical_ - 1][0] = static_cast <unsigned char> (200);

		frameReady_ = true;
		condVarTick.notify_one();
		condVarAddFrame.wait(lck, [this] {return frameReady_ == false; });
	}
}



void Snake::addSnake()
{
	while (!endGame_) {
		std::unique_lock<std::mutex> lck(mtx);

		moveSnake();

		map_ [snakeBody_[0].first] [snakeBody_[0].second] = static_cast <unsigned char> (254);

		for (int i = 1; i < snakeBody_.size(); ++i) {
			if (i < snakeBody_.size() - 1) {
				if (snakeBody_[i - 1].first < snakeBody_[i].first && snakeBody_[i - 1].second == snakeBody_[i].second) {

					if (snakeBody_[i + 1].first > snakeBody_[i].first &&  snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (179);

					if (snakeBody_[i + 1].first == snakeBody_[i].first &&  snakeBody_[i + 1].second < snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (217);

					if (snakeBody_[i + 1].first == snakeBody_[i].first &&  snakeBody_[i + 1].second > snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (192);
				}
				else if (snakeBody_[i - 1].first == snakeBody_[i].first && snakeBody_[i - 1].second > snakeBody_[i].second) {

					if (snakeBody_[i + 1].first == snakeBody_[i].first && snakeBody_[i + 1].second < snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (196);

					if (snakeBody_[i + 1].first < snakeBody_[i].first && snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (192);

					if (snakeBody_[i + 1].first > snakeBody_[i].first && snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (218);
				}
				else if (snakeBody_[i - 1].first > snakeBody_[i].first && snakeBody_[i - 1].second == snakeBody_[i].second) {

					if (snakeBody_[i + 1].first == snakeBody_[i].first && snakeBody_[i + 1].second > snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (218);

					if (snakeBody_[i + 1].first < snakeBody_[i].first && snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (179);

					if (snakeBody_[i + 1].first == snakeBody_[i].first && snakeBody_[i + 1].second < snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (191);
				}
				else if (snakeBody_[i - 1].first == snakeBody_[i].first && snakeBody_[i - 1].second < snakeBody_[i].second) {

					if (snakeBody_[i + 1].first < snakeBody_[i].first && snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (217);

					if (snakeBody_[i + 1].first == snakeBody_[i].first && snakeBody_[i + 1].second > snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (196);

					if (snakeBody_[i + 1].first > snakeBody_[i].first && snakeBody_[i + 1].second == snakeBody_[i].second)
						map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (191);
				}
			}
			else if (snakeBody_[i - 1].first != snakeBody_[i].first) {
				map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (179);
			}
			else if (snakeBody_[i - 1].second != snakeBody_[i].second) {
				map_[snakeBody_[i].first][snakeBody_[i].second] = static_cast <unsigned char> (196);
			}
		}

		snakeReady_ = true;
		condVarTick.notify_one();
		condVarAddSnake.wait(lck, [this] {return snakeReady_ == false; });
	}
}



void Snake::addFood()
{
	while (!endGame_) {
		std::vector<std::vector<int>> freeField (mapVertical_);
		int emptyPoints = 0;
		std::unique_lock<std::mutex> lck(mtx);
		for (int i = 0; i < mapVertical_; ++i) {
			for (int j = 0; j < mapHorizontal_; ++j) {
				if (map_[i][j] == ' ') {
					freeField[i].push_back(j);
					emptyPoints++;
				}
			}
		}

		if (emptyPoints > 5) {
			if (foodPosition_.first == -1 && foodPosition_.second == -1) {
				foodPosition_.first = rand() % freeField.size();
				foodPosition_.second = rand() % freeField[foodPosition_.first].size();
			}
			map_[foodPosition_.first][foodPosition_.second] = static_cast <unsigned char> (248);
		}
		else {
			endGame_ = true;
		}
	}
}



void Snake::draw()
{
	for (int i = 0; i < mapVertical_; ++i) {
		for (int j = 0; j < mapHorizontal_; ++j) {
			std::cout << map_[i][j];
		}
		std::cout << std::endl;
	}
}



void Snake::tick()
{
	std::unique_lock<std::mutex> lck(mtx);
	while (!endGame_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(150));

		nextTick_ = true;

		condVarClearMap.notify_one();
		condVarTick.wait(lck, [this] { return nextTick_ == false; });

		condVarAddFrame.notify_one();
		condVarTick.wait(lck, [this] { return frameReady_ == true; });

		condVarAddSnake.notify_one();
		condVarTick.wait(lck, [this] { return snakeReady_ == true; });

		draw();
	}

	frameReady_ = snakeReady_ = false;
	nextTick_ = true;
	condVarClearMap.notify_one();
	condVarAddFrame.notify_one();
	condVarAddSnake.notify_one();
	condVarAddFood.notify_one();
	
}

void Snake::changeDirection()
{
	const int keyUp = 72;
	const int keyDown = 80;
	const int keyLeft = 75;
	const int keyRight = 77;

	while (!endGame_) {
		int key = _getch();
		if (key == 0 || key == 0xE0) {
			switch (key = _getch()) {
			case keyUp:
				direction_ = 0;
				break;
			case keyDown:
				direction_ = 1;
				break;
			case keyRight:
				direction_ = 2;
				break;
			case keyLeft:
				direction_ = 3;
				break;
			default:
				break;
			}
		}
	}
}

void Snake::moveSnake()
{
	std::pair<int, int> oldHead = snakeBody_[0];

	switch (direction_) {
	case 0:
		snakeBody_[0].first--;
		break;
	case 1:
		snakeBody_[0].first++;
		break;
	case 2:
		snakeBody_[0].second++;
		break;
	case 3:
		snakeBody_[0].second--;
		break;
	default:
		break;
	}

	bool doGrow = false;
	std::pair<int, int> part;


	if (map_[snakeBody_[0].first][snakeBody_[0].second] == 248) {
		foodPosition_.first = foodPosition_.second = -1;
		doGrow = true;
		part = snakeBody_[snakeBody_.size() - 1];
	}

	for (int i = snakeBody_.size() - 1; i > 0; --i) {
		if (i == 1)
			snakeBody_[i] = oldHead;
		else
			snakeBody_[i] = snakeBody_[i - 1];
	}
	if (doGrow) {
		if (snakeBody_.size() <= 1)
			snakeBody_.push_back(oldHead);
		else
			snakeBody_.push_back(part);
	}

	std::pair<int, int> head = snakeBody_[0];
	for (int i = 1; i < snakeBody_.size(); ++i) {
		if (snakeBody_[i].first == head.first && snakeBody_[i].second == head.second) {
			endGame_ = true;
			break;
		}
	}

	if (head.first == 0 || head.first == mapVertical_ - 1) {
		endGame_ = true;
	}
	else if (head.second == 0 || head.second == mapHorizontal_ - 1) {
		endGame_ = true;
	}
}