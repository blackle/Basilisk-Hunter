#include "RateLimitedWorker.h"
#include <model/Configuration.h>
#include <chrono>
#include <thread>
#include <algorithm>

namespace chrono = std::chrono;

constexpr uint64_t MICROSECONDS_IN_SECOND = chrono::duration_cast<chrono::microseconds>(chrono::seconds(1)).count();

RateLimitedWorker::RateLimitedWorker(LockBox<Challenge>* box, const Configuration* config, float limit)
	: super(box, config)
	, m_limit(limit)
	, m_speedometer(this)
{}

bool RateLimitedWorker::do_batch()
{
	if (m_speedometer.seconds() > 1) {
		m_speedometer.start();
	}

	bool ret = super::do_batch();
	if (!ret) {
		return false;
	}

	float mh = m_speedometer.million_hashes();
	float seconds = m_speedometer.seconds();
	double seconds_to_wait = std::max(mh/m_limit - seconds, 0.f);
	auto microseconds_to_wait = static_cast<uint64_t>(seconds_to_wait * MICROSECONDS_IN_SECOND);

	std::this_thread::sleep_for(chrono::microseconds(microseconds_to_wait));
	return true;
}