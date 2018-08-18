#include <iostream>

#include "socket_t/socket.h"
#include "socket_t/proto_socket.h"
#include "socket_t/proto_ip.h"

#include "cmd_args.h"
#include "validate.h"
#include "stringify.h"

static void run_server(const sock::addr_ip4& addr) {
	sock::socket serv_socket{sock::addr_family::inet, sock::sock_type::stream};
	serv_socket.setsockopt<sock::proto_socket::reuseaddr>(true);
	serv_socket.bind(addr);
	serv_socket.listen(1);

	auto cli_socket = serv_socket.accept();
	std::cout << "Accepted connection from " << to_string(cli_socket.second.get_family()) << " family." << std::endl;
	if (cli_socket.second.get_family() != sock::addr_family::inet) {
		std::cerr << "Only IPv4 is supported" << std::endl;
		return;
	}
}

int main(int argc, char** argv) {

	cmd_args args{argc, argv};

	auto server_ip_arg = args.get_value("server_ip");
	auto server_port_arg = args.get_value("server_port");

	if (server_ip_arg.empty() || server_port_arg.empty()) {
		std::cerr << "Please provide -server_ip and -server_port" << std::endl;
		return 1;
	}

	if (!validate_address(server_ip_arg)) {
		std::cerr << "Invalid server address" << std::endl;
		return 1;
	}

	auto server_port = validate_port(server_port_arg);
	if (server_port < 0) {
		std::cerr << "Invalid server port" << std::endl;
		return 1;
	}

	std::cout << "Listening on: " << server_ip_arg << ":" << server_port_arg << std::endl;

	run_server({server_ip_arg, server_port});

	std::cout << "Simple TCP server done." << std::endl;

	return 0;
}
