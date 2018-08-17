#include "socket_t_test/common.h"

#include "socket_t/proto.h"
#include "socket_t/proto_socket.h"
#include "socket_t/socket.h"

static void test_db() {
	auto protos = sock::protocols::get();

	for (auto i : protos.all()) {
		assert_true(!i.get_name().empty(), "Protocol has no name");
		assert_true(i.get_number() >= 0, "Protocol has negative number");
	}

	assert_equal(protos.by_name("icmp").get_number(), 1, "icmp != 1");
	assert_equal(protos.by_number(6).get_name(), "tcp", "6 != tcp");

	assert_fail(expect_fail([&]{
		protos.by_name("not existing");
	}), "Protocol not found");
	assert_fail(expect_fail([&]{
		protos.by_number(5000);
	}), "Protocol not found");

}

static void test_socket() {
	sock::socket s{sock::addr_family::inet, sock::sock_type::stream};
	s.setsockopt<sock::proto_socket::rcvbuf>(100);
	assert_equal(s.getsockopt<sock::proto_socket::rcvbuf>(), 100, "set/get rcvbuf 100");
	s.setsockopt<sock::proto_socket::rcvbuf>(1000);
	assert_equal(s.getsockopt<sock::proto_socket::rcvbuf>(), 1000, "set/get rcvbuf 1000");
	s.setsockopt<sock::proto_socket::sndbuf>(100);
	assert_equal(s.getsockopt<sock::proto_socket::sndbuf>(), 100, "set/get sndbuf 100");
	s.setsockopt<sock::proto_socket::sndbuf>(1000);
	assert_equal(s.getsockopt<sock::proto_socket::sndbuf>(), 1000, "set/get sndbuf 1000");
}

static void run_test() {
	test_db();
	test_socket();
}

static const bool test_added = add_test({run_test, "Basic proto"});
