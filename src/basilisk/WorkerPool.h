#pragma once

#include <vector>
#include <string>

class Worker;
class Challenge;

class WorkerPool {
public:
	WorkerPool(Challenge* challenge, const std::string& impl_name, unsigned count);
	~WorkerPool();

	unsigned batches_computed() const;
	unsigned batch_size() const;

private:
	std::vector<Worker*> m_workers;
};
