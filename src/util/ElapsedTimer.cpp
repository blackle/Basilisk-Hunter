#include "ElapsedTimer.h"

ElapsedTimer::ElapsedTimer() {
	start();
}

void ElapsedTimer::start() {
	m_start = std::chrono::steady_clock::now();
}
