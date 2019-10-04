#pragma once

#include <chrono>

namespace chrono = std::chrono;

class ElapsedTimer {
public:
	ElapsedTimer();
	void start();

	template <typename T>
	unsigned elapsed() {
		auto end = std::chrono::system_clock::now();
		return chrono::duration_cast<T>(end - m_start).count();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> m_start;
};
