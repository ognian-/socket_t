#include <iostream>

#include "cmd_args.h"
#include "validate.h"

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

	std::cout << "Connecting to: " << server_ip_arg << ":" << server_port_arg << std::endl;

	std::cout << "Simple TCP client done." << std::endl;

	return 0;
}
