#include <iostream>
#include <vector>
#include <chrono>


class FPSTracker {
	unsigned __int64 prevTime;
	unsigned __int64 elapsedTime;

	unsigned __int64 lastReportingTime;
	std::vector<unsigned __int64> times;

public:
	FPSTracker() {
		prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		elapsedTime = prevTime;
		lastReportingTime = prevTime;
	}

	int getTimeElapsed() {
		unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		elapsedTime = now - prevTime;
		prevTime = now;
		
		times.push_back(elapsedTime);

		return elapsedTime;
	}

	double getFPS() {
		getTimeElapsed();
		if (elapsedTime == 0) { return 0; }
		else { return 1000 / elapsedTime; }
	}
};