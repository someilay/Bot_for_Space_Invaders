#pragma once
#include "GamePlay.h"
/*
0 - fire
1 - right
2 - left
3 [column number] - see columnn (put in memory), column number > 0 - right, column number < 0 - left
4 [row number] - see row (put in memory), start with bottom
5 - count enemy (put in memory)
6 - get laser ready (put in memory)
7 - get position (put in memory)
8 - get global counter (% 256, put in memory)
9 [jump position] - unconditional jump
10 [memory positon] [mode] [number] - put number in memory
11 [first memory positon] [mode] [second memory positon] [mode]- put second memory position to first one
12 [shift] - shift memory, shift % 128
13 [memory positon] [mode] [number] - add number to memory content
14 [first memory positon] [mode] [second memory positon] [mode] - add second memory content to first one
15 [memory positon] [mode] [number] - subtract number to memory content
16 [first memory positon] [mode] [second memory positon] [mode] - subtract second memory content to first one
17 [memory positon] [mode] [number] - multiply number to memory content
18 [first memory positon] [mode] [second memory positon] [mode] - multiply second memory content to first one
19 [memory positon] [mode] [number] - divide memory content to number
20 [first memory positon] [mode] [second memory positon] [mode] - divide first memory content to second one
21 [memory positon] [mode] [number] - find remainder of division memory content to number
22 [first memory positon] [mode] [second memory positon] [mode] - find remainder of division first memory content to second one
23 [memory positon] [mode] [number] [j operation] - execute operator if number == memory content
24 [first memory positon] [mode] [second memory positon] [mode] [j operation] - execute operator if second memory content == first one
25 [memory positon] [mode] [number] [j operation] - execute operator if number > memory content
26 [first memory positon] [mode] [second memory positon] [mode] [j operation] - execute operator if second memory content > first one
27 [memory positon] [mode] [number] [j operation] - execute operator if number < memory content
28 [first memory positon] [mode] [second memory positon] [mode] [j operation] - execute operator if second memory content < first one
others - unconditional change genpointer
*/
class Bot
{
private:
	const int MEMORY_SIZE = 128;
	const int GEN_SIZE = 256;
	unsigned char *gens;
	unsigned char *memory;
	short gen_pointer;

	int Cycle(int pos, int shift);
public:
	int score;
	Bot();
	~Bot();

	short Read(GamePlay *game);
	unsigned char* getGen();
	unsigned char* getMemory();
	short getGenPointer();
	void Reset();
	void Inherit(Bot *parent);
};