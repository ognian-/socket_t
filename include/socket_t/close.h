#ifndef SOCKET_T_CLOSE_H
#define SOCKET_T_CLOSE_H

#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "socket_t/errno_except.h"

SOCKT_NAMESPACE_BEGIN

//TODO !!! useful only for blocking stream sockets
inline void close_default(const int sock_fd) {
	if (sock_fd == -1) return;
	
	ignore_errno ie;
	
	::shutdown(sock_fd, SHUT_WR);

	timeval timeout {};
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000 * 10;
	::setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	
	char buf[8 * 1024];
	while (true) {
#ifdef MSG_NOSIGNAL
		if (::recv(sock_fd, buf, sizeof(buf), MSG_NOSIGNAL) < 1) break; //TODO Linux
#else
		if (::recv(sock_fd, buf, sizeof(buf), 0) < 1) break;
#endif
	}
	
    ::close(sock_fd);
}

SOCKT_NAMESPACE_END

#endif // SOCKET_T_CLOSE_H

