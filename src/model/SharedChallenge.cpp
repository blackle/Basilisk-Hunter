#include "SharedChallenge.h"

SharedChallenge::SharedChallenge(const Challenge& challenge)
	: super(challenge)
	, m_dirty(false)
{}

std::mutex& SharedChallenge::mutex() {
	return m_mutex;
}

const std::string& SharedChallenge::id(const std::lock_guard<std::mutex>&) const {
	return super::prefix();
}

const std::string& SharedChallenge::prefix(const std::lock_guard<std::mutex>&) const {
	return super::prefix();
}

unsigned SharedChallenge::nonce_length(const std::lock_guard<std::mutex>&) const {
	return super::nonce_length();
}

const Solution& SharedChallenge::solution(const std::lock_guard<std::mutex>&) const {
	return super::solution();
}

Solution SharedChallenge::get_new_solution() {
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_dirty) {
		m_dirty = false;
		return super::solution();
	}
	return Solution::null();
}

void SharedChallenge::reconcile_solutions(Solution& s) {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (s < super::solution()) {
		super::set_solution(s);
		m_dirty = true;
	} else {
		s = super::solution();
	}
}
