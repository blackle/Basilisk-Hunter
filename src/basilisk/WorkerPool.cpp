#include "WorkerPool.h"
#include "Worker.h"
#include "Challenge.h"
#include <thread>

WorkerPool::WorkerPool(Challenge* challenge, const std::string& impl_name, unsigned count)
{
	for (unsigned i = 0; i < count; i++) {
		auto worker = new Worker(impl_name, challenge);
		m_workers.push_back(worker);
		worker->setThread(new std::thread([worker] {
			while (true) {
				worker->do_batch();
			}
		}));
	}
}

WorkerPool::~WorkerPool()
{
	for (auto i = m_workers.begin(); i != m_workers.end(); i++) {
		auto worker = *i;
		delete worker;
	}
}

unsigned WorkerPool::batches_computed() const
{
	unsigned sum = 0;
	for (auto i = m_workers.begin(); i != m_workers.end(); i++) {
		auto worker = *i;
		sum += worker->batches();
	}
	return sum;
}

unsigned WorkerPool::batch_size() const
{
	return Worker::batch_size();
}
