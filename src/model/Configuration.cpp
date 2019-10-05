#include "Configuration.h"

Configuration::Configuration()
	: m_threads(0)
	, m_limit(0)
{}

void Configuration::set_impl(const std::string& impl)
{
	m_impl = impl;
}

const std::string& Configuration::impl() const
{
	return m_impl;
}

void Configuration::set_threads(unsigned threads)
{
	m_threads = threads;
}

unsigned Configuration::threads() const
{
	return m_threads;
}

void Configuration::set_name(const std::string& name)
{
	m_name = name;
}

const std::string& Configuration::name() const
{
	return m_name;
}

void Configuration::set_server(const std::string& server)
{
	m_server = server;
}

const std::string& Configuration::server() const
{
	return m_server;
}

void Configuration::set_limit(float limit)
{
	m_limit = limit;
}

float Configuration::limit() const
{
	return m_limit;
}
