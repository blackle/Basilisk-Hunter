#include "RateLimitedWorker.h"
#include <model/Configuration.h>
#include <chrono>
#include <thread>
#include <algorithm>

namespace chrono = std::chrono;

constexpr int MICROSECONDS_IN_SECOND = chrono::duration_cast<chrono::microseconds>(chrono::seconds(1)).count();

RateLimitedWorker::RateLimitedWorker(LockBox<Challenge>* box, const Configuration* config, float limit)
	: super(box, config)
	, m_limit(limit)
	, m_speedometer(this)
{}

RateLimitedWorker::~RateLimitedWorker()
{}

void RateLimitedWorker::do_batch()
{
	if (m_speedometer.seconds() > 1) {
		m_speedometer.start();
	}

	super::do_batch();

	float mh = m_speedometer.million_hashes();
	float seconds = m_speedometer.seconds();
	float seconds_to_wait = std::max(mh/m_limit - seconds, 0.f);
	uint64_t microseconds_to_wait = seconds_to_wait * MICROSECONDS_IN_SECOND;

	std::this_thread::sleep_for(chrono::microseconds(microseconds_to_wait));
}