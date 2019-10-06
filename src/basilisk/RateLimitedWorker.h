#include "Worker.h"
#include "HashSpeedometer.h"

class RateLimitedWorker : public Worker {
public:
	RateLimitedWorker(SharedChallenge* winner, const Configuration* config, float limit);
	virtual ~RateLimitedWorker();

protected:
	virtual void do_batch() override;

private:
	typedef Worker super;

	float m_limit;
	HashSpeedometer m_speedometer;
};
