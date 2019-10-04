#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <crypto/SHA256.h>
#include <crypto/SHA256ImplFactory.h>

class Minimizer;
class Basilisk;

class Worker {
public:
	Worker(const std::string& impl_name, const std::string& prefix, int nonce_length, Minimizer* winner);

	unsigned batches() const;
	static unsigned batch_size();

	std::mutex& mutex();
	void setThread(std::thread* thread);
	std::shared_ptr<std::thread> thread();

	void do_batch();

private:

	std::atomic_uint m_batches;
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	SHA256State m_hash;

	std::mutex m_mutex;
	std::shared_ptr<std::thread> m_thread;

	Minimizer* m_winner;
};
