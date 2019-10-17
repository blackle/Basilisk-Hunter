#pragma once

#include <chrono>

namespace chrono = std::chrono;

class ElapsedTimer {
public:
	ElapsedTimer();
	void start();

	template <typename T>
	typename T::rep elapsed() {
		auto end = std::chrono::steady_clock::now();
		return chrono::duration_cast<T>(end - m_start).count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
};
