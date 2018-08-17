#include <iostream>

#include "cmd_args.h"

int main(int argc, char** argv) {

	cmd_args args{argc, argv};

	std::cout << "Simple TCP server done." << std::endl;

	return 0;
}
