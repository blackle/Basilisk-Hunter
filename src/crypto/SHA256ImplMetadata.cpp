#include "SHA256ImplMetadata.h"

SHA256ImplMetadata::SHA256ImplMetadata()
	: m_supported(false)
	, m_working(false)
{}

void SHA256ImplMetadata::set_name(const std::string& name) {
	m_name = name;
}

const std::string& SHA256ImplMetadata::name() const {
	return m_name;
}

void SHA256ImplMetadata::set_supported(bool supported) {
	m_supported = supported;
}

bool SHA256ImplMetadata::supported() const {
	return m_supported;
}

void SHA256ImplMetadata::set_working(bool working) {
	m_working = working;
}

bool SHA256ImplMetadata::working() const {
	return m_working;
}

std::ostream& operator<<(std::ostream& os, const SHA256ImplMetadata& md) {
	os << md.name()
	   << "\t"
	   << (md.supported() ? "Yes" : "No")
	   << "\t"
	   << (md.supported() ? (md.working() ? "Yes" : "No") : "N/A");
	return os;
}
