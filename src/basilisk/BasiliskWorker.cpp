#include "BasiliskWorker.h"
#include "BasiliskWinner.h"
#include "Basilisk.h"

static constexpr unsigned BATCH_SIZE = 100000;

BasiliskWorker::BasiliskWorker(const std::string& impl_name, const std::string& prefix, int nonce_length, BasiliskWinner* winner)
	: m_batches(0)
	, m_minimum(winner->minimum())
	, m_winner(winner)
{
	m_sha.reset(SHA256ImplFactory::get_impl(impl_name));
	m_basilisk.reset(new Basilisk(m_sha.get(), prefix, nonce_length));
}

unsigned BasiliskWorker::batches() const {
	return m_batches.load();
}

unsigned BasiliskWorker::batch_size() {
	return BATCH_SIZE;
}

std::mutex& BasiliskWorker::mutex() {
	return m_mutex;
}

void BasiliskWorker::setThread(std::thread* thread) {
	m_thread.reset(thread);
}

std::shared_ptr<std::thread> BasiliskWorker::thread() {
	return m_thread;
}

void BasiliskWorker::do_batch() {
	for (unsigned i = 0; i < BATCH_SIZE; i++) {
		m_basilisk->step();

		if (m_basilisk->final_state() < m_minimum) {
			m_minimum = m_basilisk->final_state();
			std::lock_guard<std::mutex> lock(m_winner->mutex());
			m_winner->ingest(m_minimum, m_basilisk->nonce());
		}
	}
	m_batches++;
}
