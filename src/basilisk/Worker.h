#pragma once

#include <memory>
#include <atomic>
#include <thread>
#include <crypto/SHA256State.h>
#include <crypto/SHA256ImplFactory.h>
#include <model/Solution.h>
#include "Batcher.h"

class SharedChallenge;
class Challenge;
template <typename T> class LockBox;
class Configuration;
class Basilisk;

class Worker : public Batcher {
public:
	Worker(SharedChallenge* winner, const Configuration* config);
	virtual ~Worker();

	virtual unsigned batches() const override;
	virtual unsigned batch_size() const override;

	void setThread(std::thread* thread);
	std::shared_ptr<std::thread> thread();

protected:
	friend class WorkerPool;
	virtual void do_batch();

private:

	std::atomic_uint m_batches;
	const unsigned m_batch_size;
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	Solution m_solution;
	SharedChallenge* m_challenge;
	LockBox<Challenge>* m_box;

	std::shared_ptr<std::thread> m_thread;
};
