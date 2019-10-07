#pragma once

#include <mutex>

template <typename T>
class Unlocker;

template <typename T>
class LockBox {
public:
	LockBox() = delete;
	explicit LockBox(const T& value) : m_value(value) {}

	friend class Unlocker<T>;
private:
	T m_value;
	std::mutex m_mutex;
};

template <typename T>
class Unlocker {
public:
	Unlocker() = delete;
	explicit Unlocker(LockBox<T>& box) : m_box(&box), m_lock_guard(box->m_mutex) {}
	explicit Unlocker(LockBox<T>* box) : m_box(box), m_lock_guard(box->m_mutex) {}

	const T* operator->() const { return &m_box->m_value; }
	T* operator->() { return &m_box->m_value; }
	const T& operator *() const { return m_box->value; }
	T& operator *() { return m_box->value; }

private:
	LockBox<T>* m_box;
	std::lock_guard<std::mutex> m_lock_guard;
};
