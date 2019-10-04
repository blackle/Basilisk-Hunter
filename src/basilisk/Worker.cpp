#include "Worker.h"
#include "Challenge.h"
#include "Configuration.h"
#include "Basilisk.h"

static constexpr unsigned BATCH_SIZE = 100000;

Worker::Worker(Challenge* challenge, const Configuration* config)
	: m_batches(0)
	, m_hash(challenge->best_hash())
	, m_challenge(challenge)
{
	m_sha.reset(SHA256ImplFactory::get_impl(config->impl()));
	m_basilisk.reset(new Basilisk(m_sha.get(), m_challenge->prefix(), m_challenge->nonce_length()));
}

unsigned Worker::batches() const {
	return m_batches.load();
}

unsigned Worker::batch_size() {
	return BATCH_SIZE;
}

void Worker::setThread(std::thread* thread) {
	m_thread.reset(thread);
}

std::shared_ptr<std::thread> Worker::thread() {
	return m_thread;
}

void Worker::do_batch() {
	for (unsigned i = 0; i < BATCH_SIZE; i++) {
		m_basilisk->step();

		if (m_basilisk->final_state() < m_hash) {
			m_hash = m_basilisk->final_state();
			std::lock_guard<std::mutex> lock(m_challenge->mutex());
			m_challenge->nominate(m_hash, m_basilisk->nonce());
		}
	}
	m_batches++;
}
