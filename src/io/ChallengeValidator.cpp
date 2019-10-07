#include "ChallengeValidator.h"
#include <model/Challenge.h>
#include <crypto/SHA256Impl.h>
#include <basilisk/Basilisk.h>
#include <iostream>

bool ChallengeValidator::validate(const Challenge& challenge, const SHA256Impl* sha)
{
	try {
		Basilisk b(sha, challenge.prefix(), challenge.nonce_length());
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	if (challenge.solution() == Solution::null()) {
		return true;
	}

	auto hash = sha->double_hash(challenge.prefix() + challenge.solution().nonce());
	if (hash != challenge.solution().hash()) {
		std::cerr << "Solution hash does not match server's hash" << std::endl;
		return false;
	}
	if (challenge.solution().nonce().length() != challenge.nonce_length()) {
		std::cerr << "Solution nonce is not the reported nonce length" << std::endl;
		return false;
	}

	return true;
}
