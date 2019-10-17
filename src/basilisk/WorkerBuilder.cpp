#include "WorkerBuilder.h"
#include "Worker.h"
#include "RateLimitedWorker.h"
#include <model/Configuration.h>

std::shared_ptr<Worker> WorkerBuilder::build(LockBox<Challenge>* box, const Configuration* config) {
	if (config->limit() > 0) {
		float thread_limit = config->limit() / config->threads();
		return std::make_shared<RateLimitedWorker>(box, config, thread_limit);
	}
	return std::make_shared<Worker>(box, config);
}
