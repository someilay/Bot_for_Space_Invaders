#include "pch.h"
#include <iostream>
#include <ctime>
#include <thread>
#include "GamePlay.h"
#include "Bot.h"
#include "windows.h"
#include <SFML/Graphics.hpp>

void fullGame(Bot *bot) {
	GamePlay *g = new GamePlay();

	while (g->getAlive()){
		g->update(bot->Read(g));
	}

	if (g->getWin())
		bot->score = g->getScore() 
					 + std::max(0, 1500 - g->getTact()) 
					 + std::max(0, (600 - g->getNumberOfLasers()) * 5);
	else
		bot->score = g->getScore();

	g->~GamePlay();
	std::free(g);
}

int main()
{
	/*Bot *b = new Bot();
	unsigned char *gens = b->getGen();
	gens[0]  = 3;	gens[1]  = 41;	gens[2]  = 23;	gens[3] = 0;
	gens[4]  = 0;	gens[5]  = 255;	gens[6]  = 11;	gens[7] = 9;
	gens[8]  = 41;	gens[9]  = 1;	gens[10] = 246;	gens[11] = 3;
	gens[12] = 39;	gens[13] = 23;	gens[14] = 0;	gens[15] = 0;
	gens[16] = 255;	gens[17] = 0;	gens[18] = 9;	gens[19] = 22;
	gens[20] = 2;	gens[21] = 246;	gens[22] = 23;	gens[23] = 0;
	gens[24] = 0;	gens[25] = 3;	gens[26] = 39;	gens[27] = 23;
	gens[28] = 1;	gens[29] = 0;	gens[30] = 3;	gens[31] = 39;
	gens[32] = 23;	gens[33] = 127; gens[34] = 0;	gens[35] = 1;
	gens[36] = 47;	gens[37] = 9;	gens[38] = 20;	gens[39] = 0;
	gens[40] = 226;	gens[41] = 10;	gens[42] = 127;	gens[43] = 0;
	gens[44] = 1;	gens[45] = 9;	gens[46] = 22;	gens[47] = 10;
	gens[48] = 127;	gens[49] = 0;	gens[50] = 0;	gens[51] = 9;
	gens[52] = 9;	gens[53] = 0;	gens[54] = 0;	gens[55] = 0;
	
	std::thread t(fullGame, b);
	t.join();*/

	//Create population
	std::srand(time(0));
	const int POP_SIZE = 500;
	Bot **bots = new Bot*[POP_SIZE];
	int bestScore[25];
	int bestBot[25];
	for (int i = 0; i < POP_SIZE; i++){
		bots[i] = new Bot();
	}

	for (int i = 0; i < 25; i++) {
		bestBot[i] = 0;
		bestScore[i] = 0;
	}

	//
	GamePlay *g = new GamePlay();
	const int FIELD_POS_X = 5;
	const int FIELD_POS_Y = 5;
	const int WINDOW_WIDTH = 950;
	const int WINDOW_HEIGHT = 500;
	int counter = 1;
	int generation = 1;
	int speed = 0;
	bool allFrame = false;
	bool spacePressed = false;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Evolution");
	sf::Sprite *entitys_sprites = new sf::Sprite[5];
	sf::Texture *entitys_textures = new sf::Texture[5];
	sf::Texture field_texture;
	if (!field_texture.loadFromFile("Image/Field.png"))
				return EXIT_FAILURE;
	sf::Sprite field_sprite(field_texture);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text_score("0", font, 30);
	text_score.setPosition(10, WINDOW_HEIGHT - 100);
	sf::Text text_mode("Last Frame", font, 30);
	text_mode.setPosition(10, WINDOW_HEIGHT - 75);

	{
		if (!entitys_textures[0].loadFromFile("Image/Invader.png"))
			return EXIT_FAILURE;
		entitys_sprites[0] = sf::Sprite(entitys_textures[0]);

		if (!entitys_textures[1].loadFromFile("Image/InvaderShield.png"))
			return EXIT_FAILURE;
		entitys_sprites[1] = sf::Sprite(entitys_textures[1]);

		if (!entitys_textures[2].loadFromFile("Image/EnemyLaser.png"))
			return EXIT_FAILURE;
		entitys_sprites[2] = sf::Sprite(entitys_textures[2]);

		if (!entitys_textures[3].loadFromFile("Image/Player.png"))
			return EXIT_FAILURE;
		entitys_sprites[3] = sf::Sprite(entitys_textures[3]);

		if (!entitys_textures[4].loadFromFile("Image/PlayerLaser.png"))
			return EXIT_FAILURE;
		entitys_sprites[4] = sf::Sprite(entitys_textures[4]);

		field_sprite.setPosition(FIELD_POS_X, FIELD_POS_Y);
	}

	while (window.isOpen()){
		unsigned int start_time = clock();
		//
		{
			window.draw(field_sprite);

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Space && !spacePressed) {
						allFrame = !allFrame;
						spacePressed = true;
					}
				}
				if (event.type == sf::Event::KeyReleased) {
					if (event.key.code == sf::Keyboard::Space) {
						spacePressed = false;
					}
				}
			}

			if (g->getAlive())
				g->update(bots[0]->Read(g));
			else {
				if (g->getWin())
					bots[0]->score = g->getScore()
					+ std::max(0, 1500 - g->getTact())
					+ std::max(0, (800 - g->getNumberOfLasers()) * 5);
				else
					bots[0]->score = g->getScore();

				if (!allFrame) {
					text_mode.setString("Last Frame");

					g->drawField(entitys_sprites, &window, FIELD_POS_X, FIELD_POS_Y);
					text_score.setString("Tact: " + std::to_string(g->getTact()) + 
										 " Score: " + std::to_string(g->getScore()) +
										 " Lasers: " + std::to_string(g->getNumberOfLasers()));
					window.draw(text_score);
					window.draw(text_mode);
					window.display();
					window.clear();
				}	

				g->~GamePlay();
				g = new GamePlay();
				bots[0]->Reset();
				printf("Generation: %d ", generation);

				int num_of_threads = 10;
				std::thread *threads = new std::thread[num_of_threads];
				for (int i = 1; i < num_of_threads; i++)
					threads[i] = std::thread(fullGame, bots[(POP_SIZE / num_of_threads) * i]);
				for (int i = 1; i < num_of_threads; i++)
					threads[i].join();
				
				for (int i = 1; i < POP_SIZE/ num_of_threads; i++) {
					for (int j = 0; j < num_of_threads; j++)
						threads[j] = std::thread(fullGame, bots[(POP_SIZE / num_of_threads) * j + i]);
					for (int j = 0; j < num_of_threads; j++)
						threads[j].join();
					//fullGame(bots[i]);
				}
				for (int i = 0; i < num_of_threads; i++)
					threads[i].~thread();
				delete[] threads;			

				for (int i = 0; i < POP_SIZE; i++) {
					for (int j = 0; j < 25; j++) {
						if (bots[i]->score > bestScore[j]) {
							for (int k = 24; k > j; k--) {
								bestScore[k] = bestScore[k - 1];
								bestBot[k] = bestBot[k - 1];
							}

							bestScore[j] = bots[i]->score;
							bestBot[j] = i;
							break;
						}
					}
				}

				printf("Best score: %d Best bot: %d\n", bestScore[0], bestBot[0]);

				for (int i = 0; i < 25; i++) {
					for (int j = 20 * i; j < 20 * (i + 1); j++) {
						if (bestBot[i] != j)
							bots[j]->Inherit(bots[bestBot[i]]);
					}
				}

				for (int i = 0; i < 25; i++) {
					bestBot[i] = 0;
					bestScore[i] = 0;
				}

				for (int i = 0; i < POP_SIZE; i++) {
					bots[i]->Reset();
				}

				generation++;
			}

			if (allFrame) {
				text_mode.setString("All Frames");

				
				g->drawField(entitys_sprites, &window, FIELD_POS_X, FIELD_POS_Y);
				text_score.setString("Tact: " + std::to_string(g->getTact()) +
					" Score: " + std::to_string(g->getScore()) +
					" Lasers: " + std::to_string(g->getNumberOfLasers()));
				window.draw(text_score);
				window.draw(text_mode);
				window.display();
				window.clear();
			}
		}
		//
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;

		if (counter % 50 == 0)
			window.setTitle("Evolution " + std::to_string((float)(1000) / search_time) + " FPS");

		if (counter < 10000)
			counter++;
		else
			counter = 0;

		if (allFrame)
			Sleep(speed);
	}

	for (int i = 0; i < POP_SIZE; i++) {
		bots[i]->~Bot();
	}
	delete bots;

	delete[] entitys_sprites;
	delete[] entitys_textures;
	return 0;
}