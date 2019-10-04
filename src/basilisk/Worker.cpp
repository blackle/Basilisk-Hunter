#include "Worker.h"
#include "Challenge.h"
#include "Basilisk.h"

Worker::Worker(const std::string& impl_name, unsigned batch_size, Challenge* challenge)
	: m_batch_size(batch_size)
	, m_batches(0)
	, m_hash(challenge->best_hash())
	, m_challenge(challenge)
{
	m_sha.reset(SHA256ImplFactory::get_impl(impl_name));
	m_basilisk.reset(new Basilisk(m_sha.get(), m_challenge->prefix(), m_challenge->nonce_length()));
}

unsigned Worker::batches() const {
	return m_batches.load();
}

void Worker::setThread(std::thread* thread) {
	m_thread.reset(thread);
}

std::shared_ptr<std::thread> Worker::thread() {
	return m_thread;
}

void Worker::do_batch() {
	for (unsigned i = 0; i < m_batch_size; i++) {
		m_basilisk->step();

		if (m_basilisk->final_state() < m_hash) {
			m_hash = m_basilisk->final_state();
			std::lock_guard<std::mutex> lock(m_challenge->mutex());
			m_challenge->nominate(m_hash, m_basilisk->nonce());
		}
	}
	m_batches++;
}
