#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <basilisk/Basilisk.h>
#include <basilisk/BasiliskWinner.h>
#include <basilisk/BasiliskWorker.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace chrono = std::chrono;

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto best = SHA256ImplFactory::get_best_impl_name();
	if (best.empty()) {
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
			//todo: send to server
			std::cout << "New lowest nonce found:" << std::endl;
			std::cout << winner.nonce() << " " << winner.minimum() << std::endl;
		}
	}

	return 0;
}
