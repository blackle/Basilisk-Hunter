#include "Configuration.h"

Configuration::Configuration()
	: m_threads(0)
{}

void Configuration::setImpl(const std::string& impl)
{
	m_impl = impl;
}

const std::string& Configuration::impl() const
{
	return m_impl;
}

void Configuration::setThreads(unsigned threads)
{
	m_threads = threads;
}

unsigned Configuration::threads() const
{
	return m_threads;
}

void Configuration::setName(const std::string& name)
{
	m_name = name;
}

const std::string& Configuration::name() const
{
	return m_name;
}
