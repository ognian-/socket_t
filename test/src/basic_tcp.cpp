#include "socket_t_test/common.h"

#include "socket_t/socket.h"
#include "socket_t/proto_socket.h"
#include "socket_t/proto_ip.h"

static const std::string message = "Hello, world!";

static void client_thread() {
	sock::socket cli_socket{sock::addr_family::inet, sock::sock_type::stream};
	cli_socket.connect(sock::addr_ip4("127.0.0.1", 12345));
	char buf[256];
	auto size = cli_socket.recv({buf, sizeof(buf) - 1});
	buf[size] = 0;
	assert_equal(std::string{buf}, message, "Received wrong message");
}

static void server_thread() {
	sock::socket serv_socket{sock::addr_family::inet, sock::sock_type::stream};
	serv_socket.setsockopt<sock::proto_socket::reuseaddr>(true);
	serv_socket.bind(sock::addr_ip4("127.0.0.1", 12345));
	serv_socket.listen(1);
	thread_except client{client_thread};
	auto cli_socket = serv_socket.accept();
	cli_socket.first.send({message.c_str(), message.size()});
	client.join();
}

static void run_test() {
	for (int i = 0; i < 10; ++i) {
		server_thread();
	}
}

static const bool test_added = add_test({run_test, "Basic TCP"});

