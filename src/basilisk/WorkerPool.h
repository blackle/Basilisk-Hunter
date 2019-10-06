#pragma once

#include "Batcher.h"
#include <vector>
#include <string>

class Worker;
class SharedChallenge;
class Configuration;

class WorkerPool : public Batcher {
public:
	WorkerPool(SharedChallenge* challenge, const Configuration* config);
	virtual ~WorkerPool();

	virtual unsigned batches() const override;
	virtual unsigned batch_size() const override;

private:
	std::vector<Worker*> m_workers;
	const unsigned m_batch_size;
};
