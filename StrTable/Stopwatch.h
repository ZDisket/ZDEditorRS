#pragma once
#include <time.h>
#include <conio.h>
// Stopwatch
// Counts time in milliseconds 
class Stopwatch
{
public:
	Stopwatch();
	void           start();
	void           stop();
	void           reset();
	void           restart();
	bool           isRunning();
	unsigned long  getTime();
	bool           isOver(unsigned long seconds);
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;
};

