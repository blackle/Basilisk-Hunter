#pragma once

#include "Solution.h"

class Challenge {
public:
	Challenge();
	Challenge(const std::string& id, const std::string& prefix, unsigned nonce_length);

	void set_id(const std::string& id);
	const std::string& id() const;

	void set_prefix(const std::string& prefix);
	const std::string& prefix() const;

	void set_nonce_length(unsigned length);
	unsigned nonce_length() const;

	void set_solution(const Solution& solution);
	const Solution& solution() const;

private:
	Solution m_solution;

	std::string m_id;
	std::string m_prefix;
	unsigned m_nonce_length;
};
