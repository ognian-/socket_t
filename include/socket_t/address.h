#ifndef SOCKET_T_ADDRESS_H
#define SOCKET_T_ADDRESS_H

#include <cstring>

#include <sys/socket.h>

#include "socket_t/common.h"
#include "socket_t/lang.h"

SOCKT_NAMESPACE_BEGIN

enum class addr_family : sa_family_t {
	unspec = AF_UNSPEC,
	unix = AF_UNIX,
	inet = AF_INET,
	inet6 = AF_INET6
};

struct address_private;

class address {
public:

	constexpr operator addr_family() {
		return static_cast<addr_family>(m_storage.ss_family);
	}

private:
	friend struct address_private;
	
protected:
	constexpr address();
	inline address(const sockaddr* addr, socklen_t addrlen) noexcept;
	
	sockaddr_storage m_storage;
	socklen_t m_length;
};

struct address_private {
	
	static constexpr address make() {
		return address{};
	}
	
	static inline address make(const void* addr, socklen_t addrlen) noexcept {
		return address{reinterpret_cast<const sockaddr*>(addr), addrlen};
	}
	
	static constexpr const sockaddr* get_addr(const address& addr) {
		return raw_cast<const sockaddr*>(&addr.m_storage);
	}
	
	static constexpr sockaddr* get_addr(address& addr) {
		return raw_cast<sockaddr*>(&addr.m_storage);
	}

	static constexpr socklen_t get_addrlen(const address& addr) {
		return addr.m_length;
	}
	
	static constexpr socklen_t* get_addrlen_ptr(address& addr) {
		return &addr.m_length;
	}
	
	static constexpr const socklen_t* get_addrlen_ptr(const address& addr) {
		return &addr.m_length;
	}
	
};

constexpr address::address() : m_storage{}, m_length{sizeof(m_storage)} {}

address::address(const sockaddr* addr, socklen_t addrlen) noexcept : m_storage{}, m_length{addrlen} {
	memcpy(&m_storage, addr, addrlen);
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_ADDRESS_H
