#pragma once

#include "Batcher.h"
#include <vector>
#include <string>
#include <memory>

class Worker;
class Challenge;
template <typename T> class LockBox;
class Configuration;

class WorkerPool : public Batcher {
public:
	WorkerPool(LockBox<Challenge>* box, const Configuration* config);
	virtual ~WorkerPool();

	virtual unsigned batches() const override;
	virtual unsigned batch_size() const override;

private:
	std::vector<std::shared_ptr<Worker>> m_workers;
	const unsigned m_batch_size;
};
