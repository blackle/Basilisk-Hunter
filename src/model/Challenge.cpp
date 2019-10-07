#include "Challenge.h"

Challenge::Challenge()
	: m_nonce_length(0)
{}

Challenge::Challenge(const std::string& id, const std::string& prefix, unsigned nonce_length)
	: m_id(id)
	, m_prefix(prefix)
	, m_nonce_length(nonce_length)
{}

void Challenge::set_id(const std::string& id) {
	m_id = id;
}

const std::string& Challenge::id() const {
	return m_id;
}

void Challenge::set_prefix(const std::string& prefix) {
	m_prefix = prefix;
}

const std::string& Challenge::prefix() const {
	return m_prefix;
}

void Challenge::set_nonce_length(unsigned length) {
	m_nonce_length = length;
}

unsigned Challenge::nonce_length() const {
	return m_nonce_length;
}

void Challenge::set_solution(const Solution& solution) {
	m_solution = solution;
}

const Solution& Challenge::solution() const {
	return m_solution;
}
