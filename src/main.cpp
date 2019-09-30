#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256.h>
#include <Basilisk.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "NonceUtil.h"

namespace chrono = std::chrono;

struct BasiliskJob {
	BasiliskJob(int h, SHA256ImplName i) : hashes(h), impl(i) {}
	int hashes;
	SHA256ImplName impl;
};

void basilisk_thread(const BasiliskJob& job)
{
	auto compressor = SHA256ImplFactory::get_impl(job.impl).release();
	Basilisk basilisk(compressor, "basilisk|0000000000|", 64);

	for (int i = 0; i < job.hashes; i++) {
		basilisk.step();
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	auto best = SHA256ImplFactory::get_best_impl_name();
	if (best == SHA256ImplName::None) {
		return -1;
	}

	int hashes = 100000000;
	int threads = std::thread::hardware_concurrency();
	if (threads == 0) {
		threads = 1;
	}
	BasiliskJob job(hashes/threads, best);
	std::cout << "computing " << hashes << " hashes on " << threads << " threads" << std::endl;

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
	std::vector<std::thread*> my_threads;
	for (int i = 0; i < threads; i++) {
		my_threads.push_back(new std::thread(basilisk_thread, std::ref(job)));
	}

	for (int i = 0; i < threads; i++) {
		my_threads[i]->join();
	}
	chrono::time_point<chrono::system_clock> stop = chrono::system_clock::now();

	auto time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
	std::cout << "time: " << (hashes/(float)time * 1./1000.) << std::endl;

	return 0;
}
