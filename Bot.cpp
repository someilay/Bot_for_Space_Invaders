#include "pch.h"
#include "Bot.h"

int Bot::Cycle(int pos, int shift)
{
	return (pos + shift) % GEN_SIZE;
}

Bot::Bot()
{
	score = 0;
	gen_pointer = 0;
	gens = new unsigned char[GEN_SIZE];
	memory = new unsigned char[MEMORY_SIZE];

	for (int i = 0; i < MEMORY_SIZE; i++){
		memory[i] = 0;
	}
	for (int i = 0; i < GEN_SIZE; i++) {
		gens[i] = std::rand() % GEN_SIZE;
	}
}

Bot::~Bot()
{
	if (gens != nullptr) {
		delete gens;
		gens = nullptr;
	}
	if (memory != nullptr) {
		delete memory;
		memory = nullptr;
	}
}

short Bot::Read(GamePlay *game){
	int WIDTH = game->getWidth();
	int HEIGHT = game->getHeigth();
	char * field = game->getField();
	short counter = 0;
	int g_counter = game->getTact();

	//mode 0 - direct reading, 1 - from memory cell

	while (counter < 64) {
		switch (gens[gen_pointer])
		{
			case 0:
				gen_pointer = this->Cycle(gen_pointer, 1);
				return 0;
			case 1:
				gen_pointer = this->Cycle(gen_pointer, 1);
				return 1;//Right
			case 2:
				gen_pointer = this->Cycle(gen_pointer, 1);
				return 2;//Left
			case 3: {
				int pos_x = 0;
				int cur_x = gens[this->Cycle(gen_pointer, 1)] % (2 * WIDTH + 1) - WIDTH;

				for (int x = 0; x < WIDTH; x++) {
					if (field[(HEIGHT - 1)*WIDTH + x] == 4) {
						pos_x = x;
						break;
					}
				}

				if (pos_x + cur_x < WIDTH && pos_x + cur_x >= 0) {
					for (int y = HEIGHT - 1; y >= 0; y--) {
						memory[HEIGHT - 1 - y] = field[y * WIDTH + pos_x + cur_x];
					}
				}
				else {
					memory[0] = 255;
				}

				gen_pointer = this->Cycle(gen_pointer, 2);
				break;
			}
			case 4: {
				int cur_x = gens[this->Cycle(gen_pointer, 1)] % HEIGHT;

				for (int x = 0; x < WIDTH; x++) {
					memory[x] = field[cur_x * WIDTH + x];
				}

				gen_pointer = this->Cycle(gen_pointer, 2);
				break;
			}
			case 5: {
				int count = 0;
				for (int y = 0; y < HEIGHT; y++){
					for (int x = 0; x < WIDTH; x++){
						if (field[y * WIDTH + x] == 1 || field[y * WIDTH + x] == 2) {
							count++;
						}
					}
				}

				memory[0] = count;
				gen_pointer = this->Cycle(gen_pointer, 1);
				break;
			}
			case 6: {
				memory[0] = game->getLaserReady();
				gen_pointer = this->Cycle(gen_pointer, 1);
				break;
			}	
			case 7: {
				int pos_x = 0;

				for (int x = 0; x < WIDTH; x++) {
					if (field[(HEIGHT - 1)*WIDTH + x] == 4) {
						pos_x = x;
						break;
					}
				}

				memory[0] = pos_x;
				gen_pointer = this->Cycle(gen_pointer, 1);
				break;
			}
			case 8: {
				memory[0] = g_counter % GEN_SIZE;
				gen_pointer = this->Cycle(gen_pointer, 1);
				break;
			}
			case 9: {
				gen_pointer = gens[this->Cycle(gen_pointer, 1)];
				break;
			}
			case 10: {
				int mp = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];

				if (mode == 0) {
					memory[mp] = number;
				}
				else {
					memory[memory[mp] % MEMORY_SIZE] = number;
				}

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 11: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				memory[mp_1] = memory[mp_2];

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 12: {
				int shift = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;

				int temp1 = memory[0];
				int i = 0;
				int j = 0;
				while (i < MEMORY_SIZE) {
					int temp2 = memory[(j + shift) % MEMORY_SIZE];
					memory[(j + shift) % MEMORY_SIZE] = temp1;
					temp1 = temp2;
					j = (j + shift) % MEMORY_SIZE;
					i++;
				}

				gen_pointer = this->Cycle(gen_pointer, 2);
				break;
			}
			case 13: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];
				
				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				memory[mp_1] += number;

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 14: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				memory[mp_1] += memory[mp_2];

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 15: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				memory[mp_1] -= number;

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 16: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				memory[mp_1] -= memory[mp_2];

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 17: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				memory[mp_1] *= number;

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 18: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				memory[mp_1] *= memory[mp_2];

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 19: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (number != 0)
					memory[mp_1] /= number;
				else
					memory[mp_1] = 255;

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 20: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				if (memory[mp_2] != 0)
					memory[mp_1] /= memory[mp_2];
				else
					memory[mp_1] = 255;

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 21: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (number != 0)
					memory[mp_1] %= number;
				else
					memory[mp_1] = 255;

				gen_pointer = this->Cycle(gen_pointer, 4);
				break;
			}
			case 22: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				if (memory[mp_2] != 0)
					memory[mp_1] %= memory[mp_2];
				else
					memory[mp_1] = 255;

				gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 23: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];
				int jump = gens[this->Cycle(gen_pointer, 4)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (number == memory[mp_1])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 24: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;
				int jump = gens[this->Cycle(gen_pointer, 5)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				if (memory[mp_1] == memory[mp_2])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 6);
				break;
			}
			case 25: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];
				int jump = gens[this->Cycle(gen_pointer, 4)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (number > memory[mp_1])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 26: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;
				int jump = gens[this->Cycle(gen_pointer, 5)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				if (memory[mp_1] < memory[mp_2])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 6);
				break;
			}
			case 27: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int number = gens[this->Cycle(gen_pointer, 3)];
				int jump = gens[this->Cycle(gen_pointer, 4)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (number < memory[mp_1])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 5);
				break;
			}
			case 28: {
				int mp_1 = gens[this->Cycle(gen_pointer, 1)] % MEMORY_SIZE;
				int mode_1 = gens[this->Cycle(gen_pointer, 2)] % 2;
				int mp_2 = gens[this->Cycle(gen_pointer, 3)] % MEMORY_SIZE;
				int mode_2 = gens[this->Cycle(gen_pointer, 4)] % 2;
				int jump = gens[this->Cycle(gen_pointer, 5)];

				if (mode_1 == 1) {
					mp_1 = memory[mp_1] % MEMORY_SIZE;
				}

				if (mode_2 == 1) {
					mp_2 = memory[mp_2] % MEMORY_SIZE;
				}

				if (memory[mp_1] > memory[mp_2])
					gen_pointer = jump;
				else
					gen_pointer = this->Cycle(gen_pointer, 6);
				break;
			}
			default: {
				int jump = gens[gen_pointer];
				gen_pointer = this->Cycle(gen_pointer, jump);
				break;
			}
		}

		counter++;
	}
	return 4;
}

unsigned char * Bot::getGen()
{
	return gens;
}

unsigned char * Bot::getMemory()
{
	return memory;
}

short Bot::getGenPointer()
{
	return gen_pointer;
}

void Bot::Reset(){
	score = 0;
	gen_pointer = 0;
	for (int i = 0; i < MEMORY_SIZE; i++) {
		memory[i] = 0;
	}
}

void Bot::Inherit(Bot * parent){
	for (int i = 0; i < GEN_SIZE; i++){
		if (std::rand() % 10 != 0){
			gens[i] = parent->gens[i];
		}
		else {
			gens[i] = parent->gens[i];
			gens[i] += std::rand() % 11 - 5;
		}
	}
}
