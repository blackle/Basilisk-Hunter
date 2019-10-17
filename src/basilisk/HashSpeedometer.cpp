#include "HashSpeedometer.h"
#include "Batcher.h"

HashSpeedometer::HashSpeedometer(const Batcher* batcher)
	: m_batcher(batcher)
	, m_batches(batcher->batches())
{}

void HashSpeedometer::start()
{
	m_timer.start();
	m_batches = m_batcher->batches();
}

float HashSpeedometer::seconds() {
	return m_timer.elapsed<chrono::milliseconds>() / 1000.0f;
}

float HashSpeedometer::million_hashes() {
	return static_cast<float>(m_batcher->batches() - m_batches) * m_batcher->batch_size() / 1000000.0f;
}

float HashSpeedometer::million_hashes_per_second()
{
	return (million_hashes() / seconds());
}
