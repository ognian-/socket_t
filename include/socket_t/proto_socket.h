#ifndef SOCKET_T_PROTO_SOCKET_H
#define SOCKET_T_PROTO_SOCKET_H

#include <chrono>

#include <sys/socket.h>
#include <sys/time.h>

#include "socket_t/sockopt.h"

SOCKT_NAMESPACE_BEGIN

class timeout_t {
public:
	template<typename Rep, typename Period>
	constexpr timeout_t(const std::chrono::duration<Rep, Period>& time);
	constexpr timeout_t(const ::timeval& time);

	constexpr operator ::timeval() const;
	constexpr operator const std::chrono::microseconds&() const;

	constexpr bool operator==(const timeout_t& other) const;
	constexpr bool operator!=(const timeout_t& other) const;

private:
	const std::chrono::microseconds m_time;
};

class linger_t {
public:
	constexpr linger_t(int seconds);
	constexpr linger_t(const ::linger& in);

	constexpr operator ::linger() const;

	constexpr bool get_enabled() const;
	constexpr int get_seconds() const;

private:
	const bool m_enabled;
	const int m_seconds;
};

class proto_socket {
	~proto_socket() = delete;
public:

	using acceptconn = sockopt<SOL_SOCKET, SO_ACCEPTCONN, int, true, false, bool>;
	using reuseaddr = sockopt<SOL_SOCKET, SO_REUSEADDR, int, false, true, bool>;
	using rcvbuf = sockopt<SOL_SOCKET, SO_RCVBUF, int, true, true>;
	using sndbuf = sockopt<SOL_SOCKET, SO_SNDBUF, int, true, true>;
	using rcvtimeo = sockopt<SOL_SOCKET, SO_RCVTIMEO, ::timeval, true, true, timeout_t>;
	using sndtimeo = sockopt<SOL_SOCKET, SO_SNDTIMEO, ::timeval, true, true, timeout_t>;
	using linger = sockopt<SOL_SOCKET, SO_LINGER, ::linger, true, true, linger_t>;

};

template<typename Rep, typename Period>
constexpr timeout_t::timeout_t(const std::chrono::duration<Rep, Period>& time) :
	m_time{time} {}

constexpr timeout_t::timeout_t(const ::timeval& time) :
	timeout_t{std::chrono::seconds(time.tv_sec) + std::chrono::microseconds(time.tv_usec)} {}

constexpr timeout_t::operator ::timeval() const {
	::timeval out{};
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(m_time);
	out.tv_sec  = sec.count();
	out.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(m_time - sec).count();
	return out;
}

constexpr timeout_t::operator const std::chrono::microseconds&() const {
	return m_time;
}

constexpr bool timeout_t::operator==(const timeout_t& other) const {
	return m_time == other.m_time;
}

constexpr bool timeout_t::operator!=(const timeout_t& other) const {
	return m_time != other.m_time;
}

constexpr linger_t::linger_t(int seconds) : m_enabled{seconds >= 0}, m_seconds{seconds >= 0 ? seconds : 0} {}

constexpr linger_t::linger_t(const ::linger& in) : m_enabled{in.l_linger != 0}, m_seconds{in.l_linger} {}

constexpr linger_t::operator ::linger() const {
	::linger out{};
	out.l_onoff = m_enabled ? 1 : 0;
	out.l_linger = m_seconds;
	return out;
}

constexpr bool linger_t::get_enabled() const {
	return m_enabled;
}

constexpr int linger_t::get_seconds() const {
	return m_seconds;
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_PROTO_SOCKET_H
