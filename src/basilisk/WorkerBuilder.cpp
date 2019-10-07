#include "WorkerBuilder.h"
#include "Worker.h"
#include "RateLimitedWorker.h"
#include <model/Configuration.h>

Worker* WorkerBuilder::build(LockBox<Challenge>* box, const Configuration* config) {
	if (config->limit() > 0) {
		float thread_limit = config->limit() / config->threads();
		return new RateLimitedWorker(box, config, thread_limit);
	} else {
		return new Worker(box, config);
	}
}
