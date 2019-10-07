#pragma once

#include <model/Solution.h>

class Challenge;
template <typename T> class LockBox;

class ChallengeOperations
{
public:
	ChallengeOperations() = delete;
	static bool get_new_solution(LockBox<Challenge>* box, Solution& s);
	static bool reconcile_solutions(LockBox<Challenge>* box, Solution& s);
};