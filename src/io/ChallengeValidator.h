#pragma once

#include <memory>

#include <crypto/SHA256Impl.h>

class Configuration;
class Challenge;

class ChallengeValidator
{
public:
	ChallengeValidator(const Configuration* config);
	bool validate(const Challenge& challenge);
private:
	const Configuration* m_config;
	std::unique_ptr<const SHA256Impl> m_sha;
};
