#pragma once

#include <memory>
#include <atomic>
#include <thread>
#include <crypto/SHA256State.h>
#include <crypto/SHA256ImplFactory.h>

class Challenge;
class Configuration;
class Basilisk;

class Worker {
public:
	Worker(Challenge* winner, const Configuration* config);

	unsigned batches() const;
	static unsigned batch_size();

	void setThread(std::thread* thread);
	std::shared_ptr<std::thread> thread();

private:
	friend class WorkerPool;
	void do_batch();

	std::atomic_uint m_batches;
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	SHA256State m_hash;
	Challenge* m_challenge;

	std::shared_ptr<std::thread> m_thread;
};
