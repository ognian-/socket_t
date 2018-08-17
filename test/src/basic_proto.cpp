#include "socket_t_test/common.h"

#include "socket_t/proto.h"

static void run_test() {
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

static const bool test_added = add_test({run_test, "Basic proto"});
