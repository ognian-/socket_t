#ifndef SOCKET_T_SOCKET_H
#define SOCKET_T_SOCKET_H

#include <atomic>
#include <utility>

#include "socket_t/lang.h"
#include "socket_t/errno_except.h"
#include "socket_t/address.h"
#include "socket_t/close.h"
#include "socket_t/sockopt.h"

SOCKT_NAMESPACE_BEGIN

enum class sock_type : int {
    stream = SOCK_STREAM,
    dgram = SOCK_DGRAM,
	raw = SOCK_RAW,
	seqpacket = SOCK_SEQPACKET
};

enum class shutdown_type : int {
	read = SHUT_RD,
	write = SHUT_WR,
	read_write = SHUT_RDWR
};

class socket {
public:
	constexpr socket();
	inline socket(addr_family domain, sock_type type, int protocol = 0); //TODO protocol
	inline socket(socket&& other) noexcept;
	inline ~socket() noexcept;

	socket(const socket&) = delete;
	socket& operator=(const socket&) = delete;

	inline socket& operator=(socket&& other);

	inline void close();
	inline void bind(const address& addr);
	inline void listen(unsigned backlog);
	inline std::pair<socket, address> accept();
	inline void connect(const address& addr);
	inline size_t recv(mem_out mem, int flags = 0); //TODO flags
	inline std::pair<size_t, address> recvfrom(mem_out mem, int flags = 0); //TODO flags
	inline size_t send(mem_in mem, int flags = 0); //TODO flags
	inline size_t sendto(mem_in mem, int flags, const address& dest); //TODO flags
	inline static std::pair<socket, socket> socketpair(addr_family domain, sock_type type, int protocol = 0); //TODO protocol
	inline address getsockname();
	inline address getpeername();
	inline void shutdown(shutdown_type how);
	template <typename T>
	inline typename T::represent getsockopt();
	template <typename T>
	inline void setsockopt(const typename T::represent& value);
	inline bool sockatmark();

private:
	constexpr explicit socket(int fd) : m_fd{fd} {};
	std::atomic<int> m_fd;
};

constexpr socket::socket() : m_fd{-1} {}

socket::socket(addr_family domain, sock_type type, int protocol) :
	m_fd{check_errno(::socket, static_cast<int>(domain), static_cast<int>(type), protocol)} {}
	
socket::socket(socket&& other) noexcept : m_fd{other.m_fd.exchange(-1)} {}

socket::~socket() noexcept {
	try {
		close();
	} catch (...) {}
}

socket& socket::operator=(socket&& other) {
	this->~socket();
	m_fd = other.m_fd.exchange(-1);
	return *this;
}

void socket::close() {
	auto fd = m_fd.exchange(-1);
	close_default(fd);
}

void socket::bind(const address& addr) {
	check_errno(::bind, m_fd.load(), address_private::get_addr(addr), address_private::get_addrlen(addr));
}

void socket::listen(unsigned backlog) {
	check_errno(::listen, m_fd.load(), backlog);
}

std::pair<socket, address> socket::accept() {
	auto addr = address_private::make();
	return {socket{check_errno(::accept, m_fd.load(), address_private::get_addr(addr), address_private::get_addrlen_ptr(addr))}, addr};
}

void socket::connect(const address& addr) {
	check_errno(::connect, m_fd.load(), address_private::get_addr(addr), address_private::get_addrlen(addr));
}

size_t socket::recv(mem_out mem, int flags) {
	return check_errno(::recv, m_fd.load(), mem.buffer, mem.length, flags);
}

std::pair<size_t, address> socket::recvfrom(mem_out mem, int flags) {
	auto dest = address_private::make();
	auto size = check_errno(::recvfrom, m_fd.load(), mem.buffer, mem.length, flags, address_private::get_addr(dest), address_private::get_addrlen_ptr(dest));
	return {size, dest};
}

size_t socket::send(mem_in mem, int flags) {
	return check_errno(::send, m_fd.load(), mem.buffer, mem.length, flags);
}

size_t socket::sendto(mem_in mem, int flags, const address& dest) {
	return check_errno(::sendto, m_fd.load(), mem.buffer, mem.length, flags, address_private::get_addr(dest), address_private::get_addrlen(dest));
}

std::pair<socket, socket> socket::socketpair(addr_family domain, sock_type type, int protocol) {
	int fds[2];
	check_errno(::socketpair, static_cast<int>(domain), static_cast<int>(type), protocol, fds);
	return {socket{fds[0]}, socket{fds[1]}};
}

address socket::getsockname() {
	auto addr = address_private::make();
	check_errno(::getsockname, m_fd.load(), address_private::get_addr(addr), address_private::get_addrlen_ptr(addr));
	return addr;
}

address socket::getpeername() {
	auto addr = address_private::make();
	check_errno(::getpeername, m_fd.load(), address_private::get_addr(addr), address_private::get_addrlen_ptr(addr));
	return addr;
}

void socket::shutdown(shutdown_type how) {
	check_errno(::shutdown, m_fd.load(), static_cast<int>(how));
}

template <typename T>
typename T::represent socket::getsockopt() {
	static_assert(T::readable);
	typename T::type optval;
	socklen_t optlen = T::size;
	check_errno(::getsockopt, m_fd.load(), static_cast<int>(T::level), static_cast<int>(T::name), &optval, &optlen);
	return optval;
}

template <typename T>
void socket::setsockopt(const typename T::represent& value) {
	static_assert(T::writeable);
	typename T::type optval = static_cast<typename T::type>(value);
	check_errno(::setsockopt, m_fd.load(), T::level, T::name, &optval, T::size);
}

bool socket::sockatmark() {
	return !!check_errno(::sockatmark, m_fd.load());
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_SOCKET_H
