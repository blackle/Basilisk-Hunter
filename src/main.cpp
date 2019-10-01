#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <Basilisk.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "NonceUtil.h"
#include <mutex>
#include <memory>
#include <condition_variable>

namespace chrono = std::chrono;

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});
static constexpr int BATCH_LENGTH = 10000000;

struct BasiliskJob {
	BasiliskJob(SHA256ImplName i) : hashes(0), impl(i) {}
	int hashes;
	SHA256ImplName impl;
};

class BasiliskWorker {
public:
	BasiliskWorker(SHA256ImplName implName, const std::string& prefix, int nonce_length)
		: m_hashes(0)
		, m_minimum(DEFAULT_MINIMUM)
		, m_suspended(false)
	{
		m_sha.reset(SHA256ImplFactory::get_impl(implName));
		m_basilisk.reset(new Basilisk(m_sha.get(), prefix, nonce_length));
	}

	int hashes() const {
		return m_hashes;
	}

	std::mutex& mutex_1() {
		return m_mutex_1;
	}

	std::mutex& mutex_2() {
		return m_mutex_2;
	}

	std::condition_variable& cv() {
		return m_cv;
	}

	bool is_suspended() const {
		return m_suspended;
	}

	void suspend() {
		m_suspended = true;
		m_cv.notify_one();
	}

	void resume() {
		m_suspended = false;
		m_cv.notify_one();
	}

	std::string nonce() const {
		return m_nonce;
	}

	const SHA256State& minimum() const {
		return m_minimum;
	}

	void setThread(std::thread* thread) {
		m_thread.reset(thread);
	}

	std::shared_ptr<std::thread> thread() {
		return m_thread;
	}

	void do_batch() {
		for (int i = 0; i < BATCH_LENGTH; i++) {
			m_basilisk->step();
			m_hashes++;

			if (m_basilisk->final_state() < m_minimum) {
				m_minimum = m_basilisk->final_state();
				m_nonce = m_basilisk->nonce();
			}
		}
	}

private:

	int m_hashes;
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	SHA256State m_minimum;
	std::string m_nonce;

	std::mutex m_mutex_1;
	std::mutex m_mutex_2;
	std::condition_variable m_cv;
	bool m_suspended;
	std::shared_ptr<std::thread> m_thread;
};

void basilisk_thread(const BasiliskJob& job)
{
	auto compressor = SHA256ImplFactory::get_impl(job.impl);
	Basilisk basilisk(compressor, "basilisk:0000000000:", 64);
	SHA256State minimum(DEFAULT_MINIMUM);

	while (true) {
		basilisk.step();
		if (basilisk.final_state() < minimum) {
			minimum = basilisk.final_state();
		}
	}

	delete compressor;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto best = SHA256ImplFactory::get_best_impl_name();
	if (best == SHA256ImplName::None) {
		return -1;
	}

	int threads = std::thread::hardware_concurrency();
	if (threads <= 0) {
		threads = 1;
	}
	std::cout << "spinning up " << threads << " threads!" << std::endl;

	//todo: make worker pool to encapsulate this behaviour
	std::vector<BasiliskWorker*> workers;
	for (int i = 0; i < threads; i++) {
		//wrap parameters to BasiliskWorker in a ChallengeCampaign or something so it can be passed around
		auto worker = new BasiliskWorker(best, "basilisk:0000000000:", 64);
		workers.push_back(worker);
		worker->mutex_2().lock();
		worker->setThread(new std::thread([worker] {
			while (true) {
				{
					std::unique_lock<std::mutex> lock(worker->mutex_1());
					worker->cv().wait(lock, [&worker] { return !worker->is_suspended(); });
					worker->do_batch();
					//todo: maybe join data in this thread?
				}
				// std::this_thread::sleep_for(chrono::nanoseconds(1)); //agressively reschedule
				//todo: make a "priority mutex" using a condition variable and a mutex (i.e. if the 'priority' thread wants the lock, it will get it as soon as it can)
			}
		}));
	}

	auto start = chrono::system_clock::now();
	//worker pool start?
	for (auto i = workers.begin(); i != workers.end(); i++) {
		auto worker = *i;
		worker->mutex_2().unlock();
	}

	SHA256State global_min = DEFAULT_MINIMUM;
	std::string global_nonce;
	while (true) {
		std::this_thread::sleep_for(chrono::seconds(60));
		float mhs = 0;
		std::cout << "Joining threads..." << std::endl;
		for (unsigned i = 0; i < workers.size(); i++) {
			std::cout << "Joining thread " << (i+1) << "/" << workers.size() << std::endl;
			auto worker = workers[i];
			worker->suspend();
			std::lock_guard<std::mutex> lock(worker->mutex_1());

			auto end = chrono::system_clock::now();
			auto seconds = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;
			mhs += (worker->hashes()/seconds)/1000000.0;
			if (worker->minimum() < global_min) {
				global_min = worker->minimum();
				global_nonce = worker->nonce();
			}
			worker->resume();
		}
		std::cout << "MH/s: " << mhs << std::endl;
		std::cout << "min: " << global_nonce << " " << global_min << std::endl;
	}

	return 0;
}
