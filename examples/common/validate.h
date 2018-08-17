#ifndef EXAMPLES_COMMON_VALIDATE_H_
#define EXAMPLES_COMMON_VALIDATE_H_

#include <cstdint>
#include <limits>
#include <regex>

enum address_type : uint32_t {
	address_type_ip_v4 = 1 << 0,
	address_type_ip_v6 = 1 << 1,
	address_type_hostname = 1 << 2
};

constexpr uint32_t address_type_any = std::numeric_limits<uint32_t>::max();

inline bool validate_address(const std::string& in, uint32_t flags = address_type_any);

bool validate_address(const std::string& in, uint32_t flags) {
	static const std::regex ip_v4{"\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b"};
	static const std::regex ip_v6{"(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))"};
	static const std::regex hostname{"(?=^.{1,254}$)(^(?:(?!\\d+.|-)[a-zA-Z0-9_\\-]{1,62}[a-zA-Z0-9]\\.?)+(?:[a-zA-Z]{2,})$)"};

	if (flags & address_type_ip_v4) {
		if (std::regex_match(in, ip_v4)) return true;
	}

	if (flags & address_type_ip_v6) {
		if (std::regex_match(in, ip_v6)) return true;
	}

	if (flags & address_type_hostname) {
		if (std::regex_match(in, hostname)) return true;
	}

	return false;
}

int validate_port(const std::string& in) {
	try {
		auto port = std::stoi(in);
		if (port < 1 || port > 65535) return -1;
		return port;
	} catch (const std::invalid_argument&) {
		return -1;
	} catch (const std::out_of_range&) {
		return -1;
	}
}

#endif /* EXAMPLES_COMMON_VALIDATE_H_ */
