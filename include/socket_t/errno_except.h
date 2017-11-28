#ifndef SOCKET_T_ERRNO_EXCEPT_H
#define SOCKET_T_ERRNO_EXCEPT_H

#include <cerrno>
#include <system_error>
#include <type_traits>

inline void check_errno() {
	const auto e = errno;
	if (e) {
		errno = 0;
		throw std::system_error(e, std::system_category());
	}
}

template <typename Function, typename ... Args>
inline auto check_errno(Function&& function, Args&& ... args) -> typename std::result_of<Function&&(Args&& ...)>::type {
	const auto ret = function(args ...);
	check_errno();
	return ret;
}

struct ignore_errno {
	inline ignore_errno() noexcept : m_errno{errno} {}
	inline ~ignore_errno() noexcept {
		errno = m_errno;
	}
private:
	const int m_errno;
};

#endif // SOCKET_T_ERRNO_EXCEPT_H
