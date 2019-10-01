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
#include <atomic>

namespace chrono = std::chrono;

static const SHA256State DEFAULT_MINIMUM({
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
});
static constexpr unsigned BATCH_LENGTH = 100000;

struct BasiliskJob {
	BasiliskJob(SHA256ImplName i) : hashes(0), impl(i) {}
	int hashes;
	SHA256ImplName impl;
};

class BasiliskWinner {
public:
	BasiliskWinner() : m_minimum(DEFAULT_MINIMUM), m_dirty(false) {}

	void ingest(SHA256State& minimum, const std::string& nonce) {
		if (minimum < m_minimum) {
			m_minimum = minimum;
			m_nonce = nonce;
			m_dirty = true;
		} else {
			minimum = m_minimum;
		}
	}

	std::mutex& mutex() {
		return m_mutex;
	}

	bool is_dirty() const {
		return m_dirty;
	}

	void clear_dirty() {
		m_dirty = false;
	}

	const SHA256State& minimum() const {
		return m_minimum;
	}

	const std::string& nonce() const {
		return m_nonce;
	}

private:
	SHA256State m_minimum;
	std::string m_nonce;
	std::mutex m_mutex;
	bool m_dirty;
};

class BasiliskWorker {
public:
	BasiliskWorker(SHA256ImplName implName, const std::string& prefix, int nonce_length, BasiliskWinner* winner)
		: m_batches(0)
		, m_minimum(winner->minimum())
		, m_winner(winner)
	{
		m_sha.reset(SHA256ImplFactory::get_impl(implName));
		m_basilisk.reset(new Basilisk(m_sha.get(), prefix, nonce_length));
	}

	unsigned batches() const {
		return m_batches.load();
	}

	unsigned batch_size() const {
		return BATCH_LENGTH;
	}

	std::mutex& mutex() {
		return m_mutex;
	}

	void setThread(std::thread* thread) {
		m_thread.reset(thread);
	}

	std::shared_ptr<std::thread> thread() {
		return m_thread;
	}

	void do_batch() { //todo: hide in private and use friends to let the thread run it
		for (unsigned i = 0; i < BATCH_LENGTH; i++) {
			m_basilisk->step();

			if (m_basilisk->final_state() < m_minimum) {
				//todo: lock in here???
				m_minimum = m_basilisk->final_state();
				std::lock_guard<std::mutex> lock(m_winner->mutex());
				m_winner->ingest(m_minimum, m_basilisk->nonce());
			}
		}
		m_batches++;
	}

private:

	std::atomic_uint m_batches; //store as atomic that is externally accessible?
	std::shared_ptr<const SHA256Impl> m_sha;
	std::shared_ptr<Basilisk> m_basilisk;

	SHA256State m_minimum;

	std::mutex m_mutex;
	std::shared_ptr<std::thread> m_thread;

	BasiliskWinner* m_winner;
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

	BasiliskWinner winner; //todo: initialize with data from server

	//todo: make worker pool to encapsulate this behaviour
	std::vector<BasiliskWorker*> workers;
	for (int i = 0; i < threads; i++) {
		//wrap parameters to BasiliskWorker in a ChallengeCampaign or something so it can be passed around
		auto worker = new BasiliskWorker(best, "basilisk:0000000000:", 64, &winner);
		workers.push_back(worker);
		worker->mutex().lock();
		worker->setThread(new std::thread([worker] {
			while (true) {
				worker->do_batch();
			}
		}));
	}

	auto start = chrono::system_clock::now();
	//worker pool start?
	for (auto i = workers.begin(); i != workers.end(); i++) {
		auto worker = *i;
		worker->mutex().unlock();
	}

	while (true) {
		std::this_thread::sleep_for(chrono::seconds(10));
		float mhs = 0;
		for (unsigned i = 0; i < workers.size(); i++) {
			auto worker = workers[i];

			auto end = chrono::system_clock::now();
			float ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
			mhs += (worker->batches()/ms)/1000.0 * worker->batch_size(); //todo: change this from cumulative average to moving average
		}
		std::cout << "MH/s: " << mhs << std::endl;

		std::lock_guard<std::mutex> lock(winner.mutex());
		if (winner.is_dirty()) {
			winner.clear_dirty();
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << winner.nonce() << " " << winner.minimum() << std::endl;
		}
	}

	return 0;
}
