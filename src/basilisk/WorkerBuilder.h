#pragma once

class Challenge;
template <typename T> class LockBox;
class Configuration;
class Worker;

class WorkerBuilder
{
public:
	WorkerBuilder() = delete;
	static Worker* build(LockBox<Challenge>* box, const Configuration* config);
};