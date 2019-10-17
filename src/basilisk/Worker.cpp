#include "Worker.h"
#include "ChallengeOperations.h"
#include <model/Configuration.h>
#include <model/Challenge.h>
#include <util/LockBox.h>
#include "Basilisk.h"

Worker::Worker(LockBox<Challenge>* box, const Configuration* config)
	: m_batches(0)
	, m_batch_size(config->batch_size())
	, m_box(box)
	, m_running(true)
{
	const Unlocker<Challenge> challenge(m_box);
	m_sha = SHA256ImplFactory::get_impl(config->impl());
	m_basilisk.reset(new Basilisk(m_sha.get(), challenge->prefix(), challenge->nonce_length()));
}

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

void Worker::terminate() {
	m_running = false;
	m_thread->join();
}

bool Worker::do_batch() {
	for (unsigned i = 0; i < m_batch_size; i++) {
		m_basilisk->step();

		if (m_basilisk->final_state() < m_solution.hash()) {
			m_solution = Solution(m_basilisk->final_state(), m_basilisk->nonce());

			ChallengeOperations::reconcile_solutions(m_box, m_solution);
		}
	}
	m_batches++;
	return m_running;
}
