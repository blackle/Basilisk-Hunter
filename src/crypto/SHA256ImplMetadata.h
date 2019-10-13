#pragma once

#include <string>

class SHA256ImplMetadata {
public:
	SHA256ImplMetadata();

	void set_name(const std::string& name);
	const std::string& name() const;

	void set_supported(bool supported);
	bool supported() const;

	void set_working(bool working);
	bool working() const;

private:
	std::string m_name;
	bool m_supported;
	bool m_working;
};
