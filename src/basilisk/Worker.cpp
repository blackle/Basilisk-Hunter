#include "Worker.h"
#include <model/SharedChallenge.h>
#include <model/Configuration.h>
#include "Basilisk.h"

Worker::Worker(SharedChallenge* challenge, const Configuration* config)
	: m_batches(0)
	, m_batch_size(config->batch_size())
	, m_hash(challenge->best_hash())
	, m_challenge(challenge)
{
	m_sha.reset(SHA256ImplFactory::get_impl(config->impl()));
	m_basilisk.reset(new Basilisk(m_sha.get(), m_challenge->prefix(), m_challenge->nonce_length()));
}

Worker::~Worker()
{}

unsigned Worker::batches() const {
	return m_batches.load();
}

unsigned Worker::batch_size() const {
	return m_batch_size;
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
