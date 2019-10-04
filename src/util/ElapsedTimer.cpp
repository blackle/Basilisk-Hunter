#include "ElapsedTimer.h"

ElapsedTimer::ElapsedTimer() {
	reset();
}

void ElapsedTimer::reset() {
	m_start = std::chrono::system_clock::now();
}
