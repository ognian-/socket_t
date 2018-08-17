#ifndef SOCKET_T_PROTO_SOCKET_H
#define SOCKET_T_PROTO_SOCKET_H

#include <sys/socket.h>

#include "socket_t/sockopt.h"

SOCKT_NAMESPACE_BEGIN

class proto_socket {
	~proto_socket() = delete;
public:

	using acceptconn = sockopt<SOL_SOCKET, SO_ACCEPTCONN, int, true, false>;
	using reuseaddr = sockopt<SOL_SOCKET, SO_REUSEADDR, int, false, true>;
	using rcvbuf = sockopt<SOL_SOCKET, SO_RCVBUF, int, true, true>;
	using sndbuf = sockopt<SOL_SOCKET, SO_SNDBUF, int, true, true>;

};

SOCKT_NAMESPACE_END

#endif // SOCKET_T_PROTO_SOCKET_H
