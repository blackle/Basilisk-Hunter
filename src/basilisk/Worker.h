#pragma once

#include <memory>
#include <atomic>
#include <thread>
#include <crypto/SHA256State.h>
#include <crypto/SHA256ImplFactory.h>
#include "Batcher.h"

class Challenge;
class Configuration;
class Basilisk;

class Worker : public Batcher {
public:
	Worker(Challenge* winner, const Configuration* config);
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

	SHA256State m_hash;
	Challenge* m_challenge;

	std::shared_ptr<std::thread> m_thread;
};
