#include "Stopwatch.h"
#include <time.h>
#include <conio.h>





//////////////////////////////////////////
// class implementation:


Stopwatch::Stopwatch() {
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}


void Stopwatch::start() {
	if (!running) {
		if (resetted)
			beg = (unsigned long)clock();
		else
			beg -= end - (unsigned long)clock();
		running = true;
		resetted = false;
	}
}


void Stopwatch::stop() {
	if (running) {
		end = (unsigned long)clock();
		running = false;
	}
}


void Stopwatch::reset() {
	bool wereRunning = running;
	if (wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if (wereRunning)
		start();
}
 // Stops, reset and starts again.
void Stopwatch::restart()
{
	if (running) {
		stop();
	}
	reset();
	start();
}


bool Stopwatch::isRunning() {
	return running;
}

// Returns time in milliseconds.
unsigned long Stopwatch::getTime() {
	if (running)
		return ((unsigned long)clock() - beg);
	else
		return end - beg;
}


bool Stopwatch::isOver(unsigned long seconds) {
	return seconds >= getTime();
}
