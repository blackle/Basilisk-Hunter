#include "ChallengeValidator.h"
#include <model/Challenge.h>
#include <model/Configuration.h>
#include <crypto/SHA256ImplFactory.h>
#include <crypto/SHA256Impl.h>
#include <basilisk/Basilisk.h>
#include <iostream>

ChallengeValidator::ChallengeValidator(const Configuration* config)
	: m_config(config)
	, m_sha(SHA256ImplFactory::get_impl(m_config->impl()))
{}

bool ChallengeValidator::validate(const Challenge& challenge)
{
	try {
		Basilisk b(m_sha.get(), challenge.prefix(), challenge.nonce_length());
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	if (challenge.solution() == Solution::null()) {
		return true;
	}

	auto hash = m_sha->double_hash(challenge.prefix() + challenge.solution().nonce());
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
