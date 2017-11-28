#ifndef SOCKET_T_PROTO_IP_H
#define SOCKET_T_PROTO_IP_H

#include <stdexcept>
#include <cstring>

#include <netinet/ip.h>
#include <arpa/inet.h>

#include "socket_t/address.h"
#include "socket_t/errno_except.h"

SOCKT_NAMESPACE_BEGIN

class addr_ip4 : public address {
public:
	using port_t = in_port_t;

	inline addr_ip4(const std::string& addr, port_t port);

	inline std::string to_string() const;

	inline void set_port(port_t port) noexcept {
		auto saddr = raw_cast<sockaddr_in*>(&m_storage);
		saddr->sin_port = htons(port);
	}

};

addr_ip4::addr_ip4(const std::string& addr, port_t port) : address{} {
	auto saddr = raw_cast<sockaddr_in*>(&m_storage);
	if (check_errno(inet_pton, AF_INET, addr.c_str(), &saddr->sin_addr) == 0) {
		throw std::invalid_argument("Invalid IPv4 address: " + addr);
	}
	saddr->sin_family = AF_INET;
	saddr->sin_port = htons(port);
	m_length = sizeof(sockaddr_in);
}

std::string addr_ip4::to_string() const {
	char str[256]; //TODO magic number
	check_errno(inet_ntop, AF_INET, &raw_cast<sockaddr_in*>(&m_storage)->sin_addr, str, sizeof(str));
	return str;
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_PROTO_IP_H
