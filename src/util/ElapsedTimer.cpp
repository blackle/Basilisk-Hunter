#include "ElapsedTimer.h"

ElapsedTimer::ElapsedTimer() {
	start();
}

void ElapsedTimer::start() {
	m_start = std::chrono::system_clock::now();
}
