#include <cstdlib>

#include "socket_t_test/common.h"

static std::vector<Test>& get_all_tests() {
	static std::vector<Test> all_tests{};
	return all_tests;
}

bool add_test(const Test& test) noexcept {
	get_all_tests().push_back(test);
	return true;
}

int main(int /*argc*/, char* /*argv*/ []) {

	std::cout << "Running " << get_all_tests().size() << " tests" << std::endl;
	unsigned failed = 0;
	for (auto&& test : get_all_tests()) {
		std::cout <<"Running test: " << test.name << std::endl;
		try {
			test.entry();
		} catch (const std::runtime_error& e) {
			++failed;
			std::cout <<"Test '" << test.name << "' failed with '" << e.what() << "'" << std::endl;
		}
	}
	
	if (failed) {
		std::cout << "Failed tests: " << failed << "/" << get_all_tests().size() << std::endl;
		return EXIT_FAILURE;
	} else {
		std::cout << "All tests passed" << std::endl;
		return EXIT_SUCCESS;
	}

}
