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

	int64_t Timer::nano() { return		   duration.count(); }
	double Timer::micro() { return (double)duration.count() / (double)1e03; }
	double Timer::milli() { return (double)duration.count() / (double)1e06; }
	double Timer::sec() { return (double)duration.count() / (double)1e09; }

	std::string Timer::time(int decimals) { return stringutils::ts::fromTime(duration.count(), decimals, stringutils::ts::timeScale::NANO); }
	std::string Timer::time() { return time(2); }

	int64_t Timer::avgNano() { return		 avg; }
	double Timer::avgMicro() { return (double)avg / (double)1e03; }
	double Timer::avgMilli() { return (double)avg / (double)1e06; }
	double Timer::avgSec() { return (double)avg / (double)1e09; }

	std::string Timer::avgTime(int decimals) { return stringutils::ts::fromTime(avg, decimals, stringutils::ts::timeScale::NANO); }
	std::string Timer::avgTime() { return avgTime(2); }
}
