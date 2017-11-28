#include "socket_t_test/common.h"

#include "socket_t/proto_ip.h"

static void run_test() {
	assert_equal(sock::addr_ip4("0.0.0.0", 0).to_string(), "0.0.0.0", "addr_ip4::to_string failed");
	assert_equal(sock::addr_ip4("255.255.255.255", 0).to_string(), "255.255.255.255", "addr_ip4::to_string failed");
	assert_fail(expect_fail([]{sock::addr_ip4("hello", 0);}), "Invalid IPv4 address: hello");
}

static const bool test_added = add_test({run_test, "Basic IPv4 address"});

