#include <parrlib/time.h>

#include <chrono>

namespace gtime {
	double time = 0.0f;
	double deltaTime = 0.0f;

	std::chrono::steady_clock::time_point getTime() {
		return std::chrono::high_resolution_clock::now();
	}

	double getTimeDiff(std::chrono::steady_clock::time_point timeA, std::chrono::steady_clock::time_point timeB) {
		std::chrono::duration<double, std::nano> timeDiff = timeB - timeA;
		return timeDiff.count();
	}
}