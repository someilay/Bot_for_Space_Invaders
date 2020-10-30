#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
/*
1 - enemy
2 - shield enemy
3 - enemy laser
4 - player
5 - player laser
*/

class GamePlay
{
private:
	const int WIDTH = 40;
	const int HEIGHT = 23;
	const int START_ENEMY_POS_X = 8;
	const int START_ENEMY_POS_Y = 0;
	const int END_ENEMY_POS_X = 31;
	const int END_ENEMY_POS_Y = 9;
	const int CELL_WIDTH = 22;
	const int CELL_HEIGHT = 16;
	int totalEnemys = 0;
	char direction = 0;//0 - Right, 1 - Left
	char last_direction = 1;//0 - False, 1 - True
	bool win = false;
	bool isAlive = true;
	short laser_ready = 0;
	char *field;
	int score;
	int tact;
	int number_of_lasers;

	void clearUpdate(char *field_update);
public:
	GamePlay();
	~GamePlay();

	char *getField();
	void drawField(sf::Sprite *entitys, sf::RenderWindow *window, 
				   const int FIELD_POS_X, const int FIELD_POS_Y);
	void update(short command);
	bool getWin();
	bool getAlive();
	int getWidth();
	int getHeigth();
	short getLaserReady();
	int getScore();
	int getTact();
	int getNumberOfLasers();
};

