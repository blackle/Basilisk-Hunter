#pragma once

class SharedChallenge;
class Configuration;
class Worker;

class WorkerBuilder
{
public:
	static Worker* build(SharedChallenge* challenge, const Configuration* config);
};