#pragma once

#include <iostream>
#include <chrono>

struct Timer {
	void start();
	void stop();
	void showTimeElapsed();
	double timeElapsed;

private:
	std::chrono::high_resolution_clock::time_point timeStart;
	std::chrono::high_resolution_clock::time_point timeEnd;
};

