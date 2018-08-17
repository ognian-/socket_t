#ifndef EXAMPLES_COMMON_CMD_ARGS_H_
#define EXAMPLES_COMMON_CMD_ARGS_H_

#include <vector>
#include <string>
#include <algorithm>

class cmd_args {
public:
	inline cmd_args(int argc, char** argv);

	inline bool have_arg(const std::string& name) const;
	inline std::string get_value(const std::string& name) const;

private:
	std::vector<std::string> m_args;
};

cmd_args::cmd_args(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		m_args.push_back(argv[i]);
	}
}

bool cmd_args::have_arg(const std::string& name) const {
	return std::find(m_args.begin(), m_args.end(), std::string("-") + name) != m_args.end();
}

std::string cmd_args::get_value(const std::string& name) const {
	auto i = std::find(m_args.begin(), m_args.end(), std::string("-") + name);
	if (i == m_args.end() || ++i == m_args.end()) return "";
	return *i;
}

#endif /* EXAMPLES_COMMON_CMD_ARGS_H_ */
