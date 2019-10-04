#include "WorkerPool.h"
#include "Worker.h"
#include "Challenge.h"
#include <thread>

WorkerPool::WorkerPool(Challenge* challenge, const std::string& impl_name, unsigned batch_size, unsigned count)
	: m_batch_size(batch_size)
{
	for (unsigned i = 0; i < count; i++) {
		auto worker = new Worker(impl_name, batch_size, challenge);
		m_workers.push_back(worker);
		worker->mutex().lock();
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

void WorkerPool::resume()
{
	for (auto i = m_workers.begin(); i != m_workers.end(); i++) {
		auto worker = *i;
		worker->mutex().unlock();
	}
}

void WorkerPool::pause()
{
	for (auto i = m_workers.begin(); i != m_workers.end(); i++) {
		auto worker = *i;
		worker->mutex().lock(); //I have a sneaking suspicion that this could deadlock...
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
	return m_batch_size;
}
