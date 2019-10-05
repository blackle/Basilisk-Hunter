#pragma once
#include <util/ElapsedTimer.h>

class Batcher;

class HashSpeedometer {
public:
	HashSpeedometer(const Batcher* batcher);

	void start();

	float seconds();
	float million_hashes();
	float million_hashes_per_second();

private:
	typedef ElapsedTimer super;

	ElapsedTimer m_timer;
	const Batcher* m_batcher;
	unsigned m_batches;
};
