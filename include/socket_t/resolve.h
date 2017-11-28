#ifndef SOCKET_T_RESOLVE_H
#define SOCKET_T_RESOLVE_H

#include <cerrno>
#include <system_error>
#include <type_traits>
#include <vector>
#include <utility>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "socket_t/socket.h"

SOCKT_NAMESPACE_BEGIN

enum class gai_errc : int {
	addrfamily = EAI_ADDRFAMILY,
	again = EAI_AGAIN,
	badflags = EAI_BADFLAGS,
	fail = EAI_FAIL,
	family = EAI_FAMILY,
	memory = EAI_MEMORY,
	nodata = EAI_NODATA,
	noname = EAI_NONAME,
	service = EAI_SERVICE,
	socktype = EAI_SOCKTYPE,
	system = EAI_SYSTEM
};

namespace {

struct gai_category : std::error_category {
	inline const char* name() const noexcept override;
	inline std::string message(int e) const override;
};

const char* gai_category::name() const noexcept {
	return "gai";
}

std::string gai_category::message(int e) const {
	auto str = gai_strerror(e);
	if (!str) return "";
	return str;
}

inline std::error_code make_error_code(gai_errc e) {
	static const gai_category gai {};
	return {static_cast<int>(e), gai};
}

inline int check_gai_errc(int e) {
	if (e) {
		throw std::system_error(make_error_code(static_cast<gai_errc>(e)));
	}
	return e;
}

} // anonymous

enum class resolve_flags : int {
	v4mapped = AI_V4MAPPED,
	addrconfig = AI_ADDRCONFIG,
	numerichost = AI_NUMERICHOST,
	passive = AI_PASSIVE,
	numericserv = AI_NUMERICSERV,
	canonname = AI_CANONNAME
};

struct resolv_entry {
	sock_type type;
	int proto; //TODO int
	address addr;
};

struct resolv_info {
	std::vector<resolv_entry> list;
	std::string canon_name;
};

struct resolv_hints {
	std::vector<resolve_flags> flags;
	addr_family family = addr_family::unspec;
	sock_type type = sock_type::stream;
	int proto = 0; //TODO int

	inline resolv_hints& add_flag(resolve_flags flag) {
		flags.push_back(flag);
		return *this;
	}

	inline resolv_hints& set_family(addr_family family) {
		this->family = family;
		return *this;
	}

	inline resolv_hints& set_type(sock_type type) {
		this->type = type;
		return *this;
	}

	inline resolv_hints& set_proto(int proto) {
		this->proto = proto;
		return *this;
	}
};

inline resolv_info resolve(const std::string& node, const std::string& service,
		const resolv_hints& hints = resolv_hints().add_flag(resolve_flags::v4mapped).add_flag(resolve_flags::addrconfig)) {
	addrinfo ai;
	zero(ai);
	for (auto flag : hints.flags) ai.ai_flags |= static_cast<int>(flag);
	ai.ai_family = static_cast<int>(hints.family);
	ai.ai_socktype = static_cast<int>(hints.type);
	ai.ai_protocol = hints.proto;
	addrinfo* rai{};
	check_gai_errc(::getaddrinfo(node.empty() ? nullptr : node.c_str(), service.empty() ? nullptr : service.c_str(), &ai, &rai));
	resolv_info result;
	for (auto i = rai; i; i = i->ai_next) {
		result.list.push_back({
			static_cast<sock_type>(i->ai_socktype),
			i->ai_protocol,
			address_private::make(i->ai_addr, i->ai_addrlen)
		});
		if (i == rai && i->ai_canonname) result.canon_name = i->ai_canonname;
	}
	freeaddrinfo(rai);
	return result;
}

enum class reverse_resolve_flags : int {
	namereqd = NI_NAMEREQD,
	dgram = NI_DGRAM,
	nofqdn = NI_NOFQDN,
	numerichost = NI_NUMERICHOST,
	numericserv = NI_NUMERICSERV
};

inline std::pair<std::string, std::string> reverse_resolve(const address& addr,
		const std::vector<reverse_resolve_flags>& flags = std::vector<reverse_resolve_flags>()) {
	char host[1024], serv[1024]; //TODO magic number
	int f = 0;
	for (auto i : flags) f |= static_cast<int>(i);
	check_gai_errc(::getnameinfo(address_private::get_addr(addr),
			address_private::get_addrlen(addr),
			host, sizeof(host),
			serv, sizeof(serv), f));
	return std::pair<std::string, std::string>({host}, {serv});
}

SOCKT_NAMESPACE_END

namespace std {
	template <> struct is_error_code_enum<sock::gai_errc> : true_type {};
}

#endif // SOCKET_T_RESOLVE_H
