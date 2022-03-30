#pragma once

#include <chrono>

#ifndef time_h
#define time_h
namespace prb {
	namespace gtime {
		extern double time;
		extern double deltaTime;

		std::chrono::steady_clock::time_point getTime();

		double getTimeDiff(std::chrono::steady_clock::time_point timeA, std::chrono::steady_clock::time_point timeB);
	}
}

#endif time_h

