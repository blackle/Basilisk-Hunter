#include "Worker.h"
#include <model/SharedChallenge.h>
#include <model/Configuration.h>
#include "Basilisk.h"

static void reconcile_solutions(SharedChallenge* challenge, Solution& solution) {
	std::lock_guard<std::mutex> lock(challenge->mutex());
	if (solution < challenge->solution()) {
		challenge->set_solution(solution);
		challenge->set_dirty(true);
	} else {
		solution = challenge->solution();
	}
}

Worker::Worker(SharedChallenge* challenge, const Configuration* config)
	: m_batches(0)
	, m_batch_size(config->batch_size())
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

		if (m_basilisk->final_state() < m_solution.hash()) {
			m_solution = Solution(m_basilisk->final_state(), m_basilisk->nonce());

			reconcile_solutions(m_challenge, m_solution);
		}
	}
	m_batches++;
}
