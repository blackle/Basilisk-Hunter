#pragma once

class Challenge;
class Configuration;
class Worker;

class WorkerBuilder
{
public:
	static Worker* build(Challenge* challenge, const Configuration* config);
};