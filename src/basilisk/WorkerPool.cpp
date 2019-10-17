#include "WorkerPool.h"
#include "Worker.h"
#include "WorkerBuilder.h"
#include <model/Configuration.h>
#include <thread>
#include <numeric>
#include <algorithm>

WorkerPool::WorkerPool(LockBox<Challenge>* box, const Configuration* config)
	: m_batch_size(config->batch_size())
{
	std::generate_n(std::back_inserter(m_workers), config->threads(), [box, config] {
		auto worker = WorkerBuilder::build(box, config);
		//todo: make WorkerThread to hide this
		worker->setThread(new std::thread([worker] {
			while (worker->do_batch()) {}
		}));
		return worker;
	});
}

WorkerPool::~WorkerPool()
{
	for (const auto& worker : m_workers) {
		worker->terminate();
	}
}

unsigned WorkerPool::batches() const
{
	return std::accumulate(m_workers.begin(), m_workers.end(), 0,
		[](unsigned sum, std::shared_ptr<const Worker> worker) {
			return sum + worker->batches();
		}
	);
}

unsigned WorkerPool::batch_size() const
{
	return m_batch_size;
}
