#include "WorkerPool.h"
#include "Worker.h"
#include "WorkerBuilder.h"
#include <model/Challenge.h>
#include <model/Configuration.h>
#include <thread>

WorkerPool::WorkerPool(Challenge* challenge, const Configuration* config)
	: m_batch_size(config->batch_size())
{
	for (unsigned i = 0; i < config->threads(); i++) {
		Worker* worker = WorkerBuilder::build(challenge, config);
		m_workers.push_back(worker);
		worker->setThread(new std::thread([worker] {
			//todo: we need to be able to terminate this thread, the WorkerPool destructor will just cause a SIGABRT
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
	return m_batch_size;
}
