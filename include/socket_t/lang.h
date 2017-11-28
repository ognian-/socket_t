#ifndef SOCKET_T_LANG_H
#define SOCKET_T_LANG_H

#include <type_traits>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <functional>
#include <cstring>

struct mem_in {
	const void* buffer;
	size_t length;
};

struct mem_out {
	void* buffer;
	size_t length;
};

template <typename T>
void zero(T& obj) noexcept {
	memset(&obj, 0, sizeof(T));
}

template <typename T, typename U>
constexpr T exchange(T& target, U newval) {
	auto oldval = target;
	target = newval;
	return oldval;
}

template <typename To, typename From>
constexpr To raw_cast(From value) {
	return static_cast<To>(const_cast<void*>(static_cast<const void*>(value)));
}

template<typename T, class Enable = void>
struct enum_value_type {
    using type = T;
};

template<typename T>
struct enum_value_type<T, typename std::enable_if<std::is_enum<T>::value>::type> {
    using type = typename std::underlying_type<T>::type;
};

template <typename T>
constexpr typename std::underlying_type<T>::type to_underlying(T val) noexcept {
    return static_cast<typename std::underlying_type<T>::type>(val);
}

class thread_except {
public:
	template <typename Function, typename ... Args>
	inline explicit thread_except(Function&& f, Args&&... args) :
		m_exception{nullptr},
		m_mutex{},
		m_thread{[&] {
				try {
					f(args ...);
				} catch (...) {
					std::unique_lock<std::mutex> lock{m_mutex};
					m_exception = std::current_exception();
				}
		}} {}

	inline void join() {
		m_thread.join();
		std::unique_lock<std::mutex> lock{m_mutex};
		if (m_exception) {
			std::rethrow_exception(m_exception);
		}
	}

private:

	std::exception_ptr m_exception;
	std::mutex m_mutex;
	std::thread m_thread;

};

#endif // SOCKET_T_LANG_H
