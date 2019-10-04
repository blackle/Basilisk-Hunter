#include "WorkerPool.h"
#include "Worker.h"
#include <model/Challenge.h>
#include <model/Configuration.h>
#include <thread>

WorkerPool::WorkerPool(Challenge* challenge, const Configuration* config)
{
	for (unsigned i = 0; i < config->threads(); i++) {
		//todo: make RateLimitedWorker
		auto worker = new Worker(challenge, config);
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

unsigned WorkerPool::batches() const
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
