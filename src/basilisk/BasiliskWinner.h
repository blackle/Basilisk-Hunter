#pragma once

#include <crypto/SHA256.h>
#include <mutex>

class BasiliskWinner {
public:
	BasiliskWinner();
	std::mutex& mutex();

	void ingest(SHA256State& minimum, const std::string& nonce);
	const SHA256State& minimum() const;
	const std::string& nonce() const;

	bool is_dirty() const;
	void clear_dirty();

private:
	SHA256State m_minimum;
	std::string m_nonce;
	std::mutex m_mutex;
	bool m_dirty;
};
