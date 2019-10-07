#pragma once

#include <model/Solution.h>

class Challenge;
template <typename T> class LockBox;

class ChallengeOperations
{
public:
	ChallengeOperations() = delete;
	bool get_new_solution(LockBox<Challenge>* box, Solution& s);
	bool reconcile_solutions(LockBox<Challenge>* box, Solution& s);
};