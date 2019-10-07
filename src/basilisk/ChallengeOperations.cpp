#include "ChallengeOperations.h"
#include <model/Challenge.h>
#include <util/LockBox.h>

bool ChallengeOperations::get_new_solution(LockBox<Challenge>* box, Solution& s) {
	const Unlocker<Challenge> challenge(box);

	if (challenge->solution() != s) {
		s = challenge->solution();
		return true;
	}
	return false;
}

bool ChallengeOperations::reconcile_solutions(LockBox<Challenge>* box, Solution& s) {
	Unlocker<Challenge> challenge(box);

	if (s < challenge->solution()) {
		challenge->set_solution(s);
		return true;
	} else {
		s = challenge->solution();
		return false;
	}
}
