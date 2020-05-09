#pragma once

#include <chrono>
#include <vector>

#include "stringutils.h"

class Timer {
public:
	//std::chrono::time_point<std::chrono::steady_clock> lastGet;
	//std::chrono::time_point<std::chrono::steady_clock> now;
	std::chrono::high_resolution_clock::time_point last;
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::duration<int64_t, std::nano> duration;

	std::vector<int64_t> avgs;
	int avgSampleSize=0;
	int64_t avg=0;
	int avgResizePointer=0;
	int avgPointer = 0;
	void clearAvgs();

	Timer();
	Timer(int avgSampleSize);

	~Timer();

	void reset();
	void set();

	int64_t getNano();
	double getMicro();
	double getMilli();
	double getSec();
	std::string getTime(int decimals);
	std::string getTime();

	int64_t getAvgNano();
	double getAvgMicro();
	double getAvgMilli();
	double getAvgSec();
	std::string getAvgTime(int decimals);
	std::string getAvgTime();
};
