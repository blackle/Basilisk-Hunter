#include "Worker.h"
#include "Minimizer.h"
#include "Basilisk.h"

static constexpr unsigned BATCH_SIZE = 100000;

Worker::Worker(const std::string& impl_name, const std::string& prefix, int nonce_length, Minimizer* winner)
	: m_batches(0)
	, m_hash(winner->hash())
	, m_winner(winner)
{
	m_sha.reset(SHA256ImplFactory::get_impl(impl_name));
	m_basilisk.reset(new Basilisk(m_sha.get(), prefix, nonce_length));
}

unsigned Worker::batches() const {
	return m_batches.load();
}

unsigned Worker::batch_size() {
	return BATCH_SIZE;
}

std::mutex& Worker::mutex() {
	return m_mutex;
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
			std::lock_guard<std::mutex> lock(m_winner->mutex());
			m_winner->nominate(m_hash, m_basilisk->nonce());
		}
	}
	m_batches++;
}
