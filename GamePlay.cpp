#include "pch.h"
#include "GamePlay.h"


void GamePlay::clearUpdate(char * field_update){
	int lenght = WIDTH * HEIGHT;

	for (int i = 0; i < lenght; i++) {
		field_update[i] = 0;
	}
}

GamePlay::GamePlay(){
	std::srand(time(0));
	number_of_lasers = 0;
	int lenght = WIDTH * HEIGHT;
	direction = 0;
	last_direction = 0;
	win = false;
	isAlive = true;
	laser_ready = 0;
	field = new char[lenght];
	score = 0;
	tact = 1;

	for (int i = 0; i < lenght; i++){
		field[i] = 0;
	}

	for (int y = 0; y < HEIGHT; y++){
		for (int x = 0; x < WIDTH; x++){
			if (y >= START_ENEMY_POS_Y && y < END_ENEMY_POS_Y &&
				x >= START_ENEMY_POS_X && x < END_ENEMY_POS_X &&
				(y - START_ENEMY_POS_Y) % 2 == 0 &&
				(x - START_ENEMY_POS_X) % 2 == 0) {
					totalEnemys++;
					if ((y - START_ENEMY_POS_Y) / 2 == (END_ENEMY_POS_Y - START_ENEMY_POS_Y + 1) / 2 - 1) {
						field[y * WIDTH + x] = 2;//Shield enemy
					}
					else {
						field[y * WIDTH + x] = 1;//Enemy
					}
			}
		}
	}

	field[WIDTH / 2 + (HEIGHT - 1) * WIDTH] = 4;
}

GamePlay::~GamePlay(){
	if (field != NULL) {
		std::free(field);
		field = NULL;
	}
}

char * GamePlay::getField(){
	return field;
}

void GamePlay::drawField(sf::Sprite *entitys, sf::RenderWindow *window, 
					     const int FIELD_POS_X, const int FIELD_POS_Y){
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (field[y * WIDTH + x] != 0) {
				entitys[(int)field[y * WIDTH + x] - 1].setPosition(1 + FIELD_POS_X + (CELL_WIDTH + 1) * x,
																   1 + FIELD_POS_Y + (CELL_HEIGHT + 1) * y);
				window->draw(entitys[(int)field[y * WIDTH + x] - 1]);
			}
		}
	}
}

void GamePlay::update(short command){
	int lenght = WIDTH * HEIGHT;
	char *field_update = new char[lenght];
	clearUpdate(field_update);

	char change_direction = 0;
	int countEnemy = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (field[y * WIDTH + x] == 1 || field[y * WIDTH + x] == 2) {
				countEnemy++;

				if (x == 0) 
					change_direction = 1;

				if (x == WIDTH - 1)
					change_direction = 1;
			}
		}
	}

	int speed_of_enemy = 2 + 10 * countEnemy / totalEnemys;
	//Updating position of enemys
	if (tact % speed_of_enemy == 0 && (change_direction == 0 || last_direction != direction)) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if ((field[y * WIDTH + x] == 1 || field[y * WIDTH + x] == 2) && 
					field_update[y * WIDTH + x] == 0) {
					if (direction == 0) {
						if (!(field[y * WIDTH + x + 1] == 5))
							field[y * WIDTH + x + 1] = field[y * WIDTH + x];
						else if (field[y * WIDTH + x] == 2)
							field[y * WIDTH + x + 1] = 1;
						else
							field[y * WIDTH + x + 1] = 0;

						field_update[y * WIDTH + x + 1] = 1;
					}
					else {
						if (!(field[y * WIDTH + x - 1] == 5))
							field[y * WIDTH + x - 1] = field[y * WIDTH + x];
						else if (field[y * WIDTH + x] == 2)
							field[y * WIDTH + x - 1] = 1;
						else
							field[y * WIDTH + x - 1] = 0;
						
						field_update[y * WIDTH + x - 1] = 1;
					}

					field[y * WIDTH + x] = 0;
					field_update[y * WIDTH + x] = 1;

					if (y == HEIGHT - 2) {
						isAlive = false;
						delete[] field_update;
						return;
					}
				}
			}
		}
		last_direction = direction;
	}
	else if (tact % speed_of_enemy == 0 && change_direction == 1) {
		if (direction == 0)
			direction = 1;
		else
			direction = 0;

		int lowest = 0;
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if ((field[y * WIDTH + x] == 1 || field[y * WIDTH + x] == 2) &&
					field_update[y * WIDTH + x] == 0 && y < HEIGHT - 1) {
					if (!(field[(y + 1) * WIDTH + x] == 5)) {
						field[(y + 1) * WIDTH + x] = field[y * WIDTH + x];
						if (lowest < y + 1)
							lowest = y + 1;
					}
					else if (field[y * WIDTH + x] == 2) {
						field[(y + 1) * WIDTH + x] = 1;
						if (lowest < y + 1)
							lowest = y + 1;
					}	
					else
						field[(y + 1) * WIDTH + x] = 0;

					field[y * WIDTH + x] = 0;
					field_update[y * WIDTH + x] = 1;
					field_update[(y + 1) * WIDTH + x] = 1;
				}
			}
		}

		score = std::max(0, score - (lowest - END_ENEMY_POS_Y + 1)*25);
	}

	//Cheaking for win
	if (countEnemy == 0) {
		win = true;
		isAlive = false;
		delete[] field_update;
		return;
	}
	clearUpdate(field_update);

	//Update enemy laser
	if (tact % 3 == 0) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (field[y * WIDTH + x] == 3 && field_update[y * WIDTH + x] == 0){
					if (y < HEIGHT - 1) {
						if (field[(y + 1) * WIDTH + x] == 0) {
							field[(y + 1) * WIDTH + x] = field[y * WIDTH + x];
							field_update[(y + 1) * WIDTH + x] = 1;
						}
						else if (field[(y + 1) * WIDTH + x] == 5) {
							field[(y + 1) * WIDTH + x] = 0;
							field_update[(y + 1) * WIDTH + x] = 1;
							score += 50;
						}
						else if (field[(y + 1) * WIDTH + x] == 4) {
							field[(y + 1) * WIDTH + x] = 0;
							field_update[(y + 1) * WIDTH + x] = 1;
							isAlive = false;
							delete[] field_update;
							return;
						}
					}

					field[y * WIDTH + x] = 0;
					field_update[y * WIDTH + x] = 1;
				}
			}
		}
	}
	clearUpdate(field_update);

	//Update laser
	if (tact % 2 == 0) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (field[y * WIDTH + x] == 5 && field_update[y * WIDTH + x] == 0) {
					if (y > 0) {
						if (field[(y - 1) * WIDTH + x] == 0) {
							field[(y - 1) * WIDTH + x] = field[y * WIDTH + x];
							field_update[(y - 1) * WIDTH + x] = 1;
						}
						else if (field[(y - 1) * WIDTH + x] == 1) {
							field[(y - 1) * WIDTH + x] = 0;
							field_update[(y - 1) * WIDTH + x] = 1;
							score += 100;
						}
						else if (field[(y - 1) * WIDTH + x] == 2) {
							field[(y - 1) * WIDTH + x] = 1;
							field_update[(y - 1) * WIDTH + x] = 1;
							score += 75;
						}
						else if (field[(y - 1) * WIDTH + x] == 3) {
							field[(y - 1) * WIDTH + x] = 0;
							field_update[(y - 1) * WIDTH + x] = 1;
							score += 50;
						}
					}

					field[y * WIDTH + x] = 0;
					field_update[y * WIDTH + x] = 1;
				}
			}
		}
	}

	//Generate enemy laser
	if (tact % 6 == 0) {
		for (int x = 0; x < WIDTH; x++){
			bool flag = true;
			for (int y = HEIGHT - 2; y >= 0; y--){
				if ((field[y * WIDTH + x] == 1 || field[y * WIDTH + x] == 2)) {
					if (rand() % 15 == 0) {
						if (field[(y + 1) * WIDTH + x] == 0) {
							field[(y + 1) * WIDTH + x] = 3;
						}
						else if (field[(y + 1) * WIDTH + x] == 5) {
							field[(y + 1) * WIDTH + x] = 0;
							score += 50;
						}
						else if (field[(y + 1) * WIDTH + x] == 4) {
							field[(y + 1) * WIDTH + x] = 0;
							isAlive = false;
							delete[] field_update;
							return;
						}
					}

					break;
				}
			}
		}
	}

	//Update player status
	if (isAlive) {
		switch (command)
		{	
			case 0: //Fire
				for (int x = 0; x < WIDTH; x++){
					if (field[(HEIGHT - 1) * WIDTH + x] == 4) {
						if (laser_ready == 0 && field[(HEIGHT - 2) * WIDTH + x] == 0) {
							field[(HEIGHT - 2) * WIDTH + x] = 5;
							laser_ready++;
							number_of_lasers++;
						}
						else if (laser_ready == 0 && field[(HEIGHT - 2) * WIDTH + x] == 3) {
							score += 50;
						}
						break;
					}
				}

				break;
			case 1://Right
				for (int x = 0; x < WIDTH; x++) {
					if (field[(HEIGHT - 1) * WIDTH + x] == 4) {
						if (x < WIDTH - 1) {
							if (field[(HEIGHT - 1) * WIDTH + x + 1] == 0) {
								field[(HEIGHT - 1) * WIDTH + x + 1] = field[(HEIGHT - 1) * WIDTH + x];
							}
							else if (field[(HEIGHT - 1) * WIDTH + x + 1] == 3) {
								field[(HEIGHT - 1) * WIDTH + x + 1] = 0;
								isAlive = false;
								delete[] field_update;
								return;
							}

							field[(HEIGHT - 1) * WIDTH + x] = 0;
						}
						
						break;
					}
				}
				break;
			case 2://Left
				for (int x = 0; x < WIDTH; x++) {
					if (field[(HEIGHT - 1) * WIDTH + x] == 4) {
						if (x > 0) {
							if (field[(HEIGHT - 1) * WIDTH + x - 1] == 0) {
								field[(HEIGHT - 1) * WIDTH + x - 1] = field[(HEIGHT - 1) * WIDTH + x];
							}
							else if (field[(HEIGHT - 1) * WIDTH + x - 1] == 3) {
								field[(HEIGHT - 1) * WIDTH + x - 1] = 0;
								isAlive = false;
								delete[] field_update;
								return;
							}

							field[(HEIGHT - 1) * WIDTH + x] = 0;
						}

						break;
					}
				}
				break;
			default:
				break;
		}
	}

	if (laser_ready < 4 && laser_ready != 0)
		laser_ready++;
	else
		laser_ready = 0;

	tact++;
	delete[] field_update;
}

bool GamePlay::getWin()
{
	return win;
}

bool GamePlay::getAlive()
{
	return isAlive;
}

int GamePlay::getWidth()
{
	return WIDTH;
}

int GamePlay::getHeigth()
{
	return HEIGHT;
}

short GamePlay::getLaserReady()
{
	return laser_ready;
}

int GamePlay::getScore()
{
	return score;
}

int GamePlay::getTact()
{
	return tact;
}

int GamePlay::getNumberOfLasers()
{
	return number_of_lasers;
}
