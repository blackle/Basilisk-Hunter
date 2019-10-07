#pragma once

class Challenge;
class SHA256Impl;

class ChallengeValidator
{
public:
	ChallengeValidator() = delete;
	static bool validate(const Challenge& challenge, const SHA256Impl* sha);
};