#ifndef SOCKET_T_TEST_COMMON_H
#define SOCKET_T_TEST_COMMON_H

#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <string>
#include <stdexcept>

struct Test {
	std::function<void()> entry;
	std::string name;
};

bool add_test(const Test& test) noexcept;

inline void assert_true(bool expr, const std::string& what) {
	if (!expr) throw std::runtime_error(what);
}

template <typename L, typename R>
inline void assert_equal(L&& l, R&& r, const std::string& what) {
	if (l != r) throw std::runtime_error(what);
}

template <typename Function, typename ... Args>
inline std::exception_ptr expect_fail(Function&& function, Args&& ... args) {
	try {
		function(args ...);
		return nullptr;
	} catch (...) {
		return std::current_exception();
	}
}

inline void assert_fail(const std::exception_ptr& except, const std::string& what) {
	if (!except) {
		throw std::runtime_error("Failed to fail: no exception caught");
	}
	try {
		std::rethrow_exception(except);
	} catch (const std::exception& e) {
		if (what != e.what()) {
			throw std::runtime_error(std::string{"Failed to fail, got '"} + e.what() + "', expected '" + what + "'");
		}
	} catch (...) {
		throw std::runtime_error("Failed with non-standard exception");
	}
}

#endif // SOCKET_T_TEST_COMMON_H
