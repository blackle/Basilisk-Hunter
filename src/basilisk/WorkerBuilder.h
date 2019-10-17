#pragma once

#include <memory>

class Challenge;
template <typename T> class LockBox;
class Configuration;
class Worker;

class WorkerBuilder
{
public:
	WorkerBuilder() = delete;
	static std::shared_ptr<Worker> build(LockBox<Challenge>* box, const Configuration* config);
};