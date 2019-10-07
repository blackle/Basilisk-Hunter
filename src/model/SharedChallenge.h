#pragma once

#include <model/Challenge.h>
#include <crypto/SHA256State.h>
#include <string>
#include <mutex>

class SharedChallenge : protected Challenge
{
public:
	SharedChallenge() = delete;
	explicit SharedChallenge(const Challenge& challenge);

	std::mutex& mutex();

	const std::string& id(const std::lock_guard<std::mutex>&) const;
	const std::string& prefix(const std::lock_guard<std::mutex>&) const;
	unsigned nonce_length(const std::lock_guard<std::mutex>&) const;
	const Solution& solution(const std::lock_guard<std::mutex>&) const;

	Solution get_new_solution();
	void reconcile_solutions(Solution& solution);

private:
	typedef Challenge super;

	bool m_dirty;
	std::mutex m_mutex;
};