#include "Timer.h"
#include <iostream>
#include <chrono>

void Timer::start() {
	this->timeStart = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
	this->timeEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> timeElapsed = timeEnd - timeStart;
	this->timeElapsed = timeElapsed.count();
}

void Timer::showTimeElapsed() {
	std::cout << "Time elapsed [ms]: " << this->timeElapsed << std::endl;
}