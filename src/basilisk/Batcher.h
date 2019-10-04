#pragma once

class Batcher
{
public:
	virtual unsigned batch_size() const = 0;
	virtual unsigned batches() const = 0;
};
