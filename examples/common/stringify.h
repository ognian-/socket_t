#ifndef EXAMPLES_COMMON_STRINGIFY_H_
#define EXAMPLES_COMMON_STRINGIFY_H_

#include <string>
#include <stdexcept>

#include "socket_t/address.h"

inline std::string to_string(sock::addr_family af);

std::string to_string(sock::addr_family af) {
	switch (af) {
	case sock::addr_family::unspec: return "AF_UNSPEC";
	case sock::addr_family::unix: return "AF_UNIX";
	case sock::addr_family::inet: return "AF_INET";
	case sock::addr_family::inet6: return "AF_INET6";
	default:
		throw std::runtime_error("Unkown address family");
	}
}

#endif /* EXAMPLES_COMMON_STRINGIFY_H_ */
