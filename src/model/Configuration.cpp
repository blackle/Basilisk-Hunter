#include "Configuration.h"

static constexpr unsigned DEFAULT_BATCH_SIZE = 10000;

Configuration::Configuration()
	: m_threads(0)
	, m_limit(0)
	, m_batch_size(DEFAULT_BATCH_SIZE)
	, m_offline(false)
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

void Configuration::set_batch_size(unsigned batch_size)
{
	m_batch_size = batch_size;
}

unsigned Configuration::batch_size() const
{
	return m_batch_size;
}

void Configuration::set_offline(bool offline)
{
	m_offline = offline;
}

bool Configuration::offline() const
{
	return m_offline;
}
