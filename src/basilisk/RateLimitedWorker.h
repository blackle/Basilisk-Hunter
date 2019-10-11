#include "Worker.h"
#include "HashSpeedometer.h"

class Challenge;
template <typename T> class LockBox;

class RateLimitedWorker : public Worker {
public:
	RateLimitedWorker(LockBox<Challenge>* box, const Configuration* config, float limit);
	virtual ~RateLimitedWorker();

protected:
	virtual bool do_batch() override;

private:
	typedef Worker super;

	float m_limit;
	HashSpeedometer m_speedometer;
};
