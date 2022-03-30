#include "Timer.h"

namespace prb {
	Timer::Timer() {
		reset();
	}

	Timer::Timer(int avgSampleSize) : Timer() {
		this->avgSampleSize = avgSampleSize;
		avgs.reserve(avgSampleSize);
	}


	Timer::~Timer()
	{
	}

	void Timer::clearAvgs() {
		avgPointer = 0;
		avgResizePointer = 0;
	}

	void Timer::reset() {
		last = std::chrono::high_resolution_clock::now();
	}

	void Timer::set() {
		now = std::chrono::high_resolution_clock::now();
		duration = now - last;

		if (avgSampleSize > 0) {
			if (avgPointer < avgSampleSize) {
				avgPointer++;
			}

			if (avgs.size() < avgSampleSize) {
				avgs.push_back(duration.count());
			}
			else {
				avgs[avgResizePointer] = avgs.back();
				avgs.back() = duration.count();

				avgResizePointer++;
				avgResizePointer %= avgPointer;
			}

			//Timer avgTimer;

			avg = 0;
			for (int i = 0; i < avgPointer; i++) {
				avg += avgs[i];
			}
			avg /= avgs.size();

			//avgTimer.set();

			//debug::rtlog << "avg. " << stringutils::getTimeFormatted(avgTimer.getNano(), 2, stringutils::TimeScale::NANO) << "\n";
		}
	}

	int64_t Timer::getNano() { return		   duration.count(); }
	double Timer::getMicro() { return (double)duration.count() / (double)1e03; }
	double Timer::getMilli() { return (double)duration.count() / (double)1e06; }
	double Timer::getSec() { return (double)duration.count() / (double)1e09; }

	std::string Timer::getTime(int decimals) { return stringutils::getTimeFormatted(duration.count(), decimals, stringutils::TimeScale::NANO); }
	std::string Timer::getTime() { return getTime(2); }

	int64_t Timer::getAvgNano() { return		 avg; }
	double Timer::getAvgMicro() { return (double)avg / (double)1e03; }
	double Timer::getAvgMilli() { return (double)avg / (double)1e06; }
	double Timer::getAvgSec() { return (double)avg / (double)1e09; }

	std::string Timer::getAvgTime(int decimals) { return stringutils::getTimeFormatted(avg, decimals, stringutils::TimeScale::NANO); }
	std::string Timer::getAvgTime() { return getAvgTime(2); }
}
