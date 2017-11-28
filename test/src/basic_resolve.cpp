#include "socket_t_test/common.h"

#include "socket_t/resolve.h"
#include "socket_t/proto_ip.h"

static void test_domain(const std::string& name) {
	try {
		auto result = sock::resolve(name, "", sock::resolv_hints().add_flag(sock::resolve_flags::canonname));
		std::cout << std::endl << "Canonical name: " << result.canon_name << std::endl;
		for (auto&& entry : result.list) {
			auto rev = reverse_resolve(entry.addr);
			std::cout << static_cast<sock::addr_ip4&>(entry.addr).to_string() << ": " << rev.first << ":" << rev.second << std::endl;
		}
	} catch (const std::exception& e) {
		std::cout << "Failed with: " << e.what() << std::endl;
	}
}

static void run_test() {
	test_domain("google.com");
	test_domain("linux.com");
	test_domain("kernel.org");
}

static const bool test_added = add_test({run_test, "Basic resolve"});
