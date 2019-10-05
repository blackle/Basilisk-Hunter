#include "WorkerBuilder.h"
#include "Worker.h"
#include "RateLimitedWorker.h"
#include <model/Configuration.h>

Worker* WorkerBuilder::build(Challenge* challenge, const Configuration* config) {
	if (config->limit() > 0) {
		float thread_limit = config->limit() / config->threads();
		return new RateLimitedWorker(challenge, config, thread_limit);
	} else {
		return new Worker(challenge, config);
	}
}
