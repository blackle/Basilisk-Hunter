#include "ChallengeValidator.h"
#include <model/Challenge.h>
#include <crypto/SHA256Impl.h>
#include <basilisk/Basilisk.h>

bool ChallengeValidator::validate(const Challenge& challenge, const SHA256Impl* sha)
{
	try {
		Basilisk b(sha, challenge.prefix(), challenge.nonce_length());
	} catch (const std::exception& e) {
		return false;
	}

	if (challenge.solution() == Solution::null()) {
		return true;
	}

	auto hash = sha->double_hash(challenge.prefix() + challenge.solution().nonce());
	if (hash != challenge.solution().hash()) {
		return false;
	}
	if (challenge.solution().nonce().length() != challenge.nonce_length()) {
		return false;
	}

	return true;
}
