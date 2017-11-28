#ifndef SOCKET_T_SOCKOPT_H
#define SOCKET_T_SOCKOPT_H

#include <cstdint>
#include <cstring>

#include "socket_t/common.h"
#include "socket_t/lang.h"

SOCKT_NAMESPACE_BEGIN

class socket;

enum class opt_level : int {
	socket = SOL_SOCKET
};

enum class opt_name : int {
	acceptconn = SO_ACCEPTCONN,
	reuseaddr = SO_REUSEADDR
};

class sockopt {
public:
	
	inline sockopt(const sockopt& other) noexcept;
	constexpr sockopt(sockopt&& other);
	inline ~sockopt() noexcept;
	
	template <typename T>
	constexpr sockopt(opt_name name, const T& val) :
		m_name{name},
		m_value{copy_value(val)},
		m_length{sizeof(val)} {}

	sockopt& operator=(const sockopt&) = delete;
	sockopt& operator=(sockopt&&) = delete;
	
	constexpr opt_name get_name() {
		return m_name;
	}

private:
	friend class socket;
	
	inline sockopt(opt_name name, const void* optval, socklen_t optlen) noexcept;
	
	template <typename T>
	static constexpr uint8_t* copy_value(const T& val) {
		auto buf = new uint8_t[sizeof(val)];
		*reinterpret_cast<T*>(buf) = val;
		return buf;
	}

	opt_name m_name;
	uint8_t* m_value = nullptr;
	socklen_t m_length;
};

sockopt::sockopt(const sockopt& other) noexcept  :
	m_name{other.m_name},
	m_value{new uint8_t[other.m_length]},
	m_length{other.m_length} {
	memcpy(m_value, other.m_value, m_length);
}

constexpr sockopt::sockopt(sockopt&& other) :
	m_name{other.m_name},
	m_value{exchange(other.m_value, nullptr)},
	m_length{other.m_length} {}

sockopt::~sockopt() noexcept {
	delete [] m_value;
}

sockopt::sockopt(opt_name name, const void* optval, socklen_t optlen) noexcept :
	m_name{name},
	m_value{new uint8_t[optlen]},
	m_length{optlen} {
	memcpy(m_value, optval, m_length);
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_SOCKOPT_H
