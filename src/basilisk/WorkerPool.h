#pragma once

#include "Batcher.h"
#include <vector>
#include <string>

class Worker;
class Challenge;

class WorkerPool : public Batcher {
public:
	WorkerPool(Challenge* challenge, const std::string& impl_name, unsigned count);
	~WorkerPool();

	virtual unsigned batches() const override;
	virtual unsigned batch_size() const override;

private:
	std::vector<Worker*> m_workers;
};
