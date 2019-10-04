#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <crypto/SHA256State.h>
#include <crypto/SHA256ImplFactory.h>

class Challenge;
class Basilisk;

class Worker {
public:
	Worker(const std::string& impl_name, unsigned batch_size, Challenge* winner);

	unsigned batches() const;

	std::mutex& mutex();
	void setThread(std::thread* thread);
	std::shared_ptr<std::thread> thread();

	void do_batch();

private:
	const unsigned m_batch_size;
	std::atomic_uint m_batches;
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	SHA256State m_hash;

	std::mutex m_mutex;
	std::shared_ptr<std::thread> m_thread;

	Challenge* m_challenge;
};
