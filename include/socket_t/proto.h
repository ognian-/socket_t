#ifndef INCLUDE_SOCKET_T_PROTO_H_
#define INCLUDE_SOCKET_T_PROTO_H_

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#include <netdb.h>

#include "common.h"
#include "lang.h"

SOCKT_NAMESPACE_BEGIN

class protocols;

class protocol {
public:

	inline int get_number() const;
	inline const std::string& get_name() const;
	inline const std::vector<const std::string>& get_aliases() const;

private:
	inline protocol(const protoent* in);

	friend class protocols;

	const int m_number;
	const std::string m_name;
	std::vector<const std::string> m_aliases;
};

class protocols {
public:

	inline static const protocols& get();
	inline const std::vector<const protocol>& all() const;
	inline const protocol& by_number(int number) const;
	inline const protocol& by_name(const std::string& name) const;

private:

	inline protocols();

	std::vector<const protocol> m_protocols;
	std::map<int, size_t> m_numbers;
	std::map<std::string, size_t> m_names;
};

int protocol::get_number() const {
	return m_number;
}

const std::string& protocol::get_name() const {
	return m_name;
}

const std::vector<const std::string>& protocol::get_aliases() const {
	return m_aliases;
}

protocol::protocol(const protoent* in) :
	m_number{in->p_proto},
	m_name{in->p_name},
	m_aliases{} {
	for (auto i = in->p_aliases; *i; ++i) {
		m_aliases.push_back(*i);
	}
}

const protocols& protocols::get() {
	static protocols protos{};
	return protos;
}

const std::vector<const protocol>& protocols::all() const {
	return m_protocols;
}

const protocol& protocols::by_number(int number) const {
	auto i = m_numbers.find(number);
	if (i == m_numbers.end()) {
		throw std::runtime_error{"Protocol not found"};
	}
	return m_protocols[i->second];
}

const protocol& protocols::by_name(const std::string& name) const {
	auto i = m_names.find(name);
	if (i == m_names.end()) {
		throw std::runtime_error{"Protocol not found"};
	}
	return m_protocols[i->second];
}

protocols::protocols() {
	finally cleanup([]{
		endprotoent();
	});
	setprotoent(1);
	while (auto i = getprotoent()) {
		m_protocols.push_back(i);
		auto pos = m_protocols.size() - 1;
		m_numbers[i->p_proto] = pos;
		m_names[i->p_name] = pos;
	}
}

SOCKT_NAMESPACE_END

#endif /* INCLUDE_SOCKET_T_PROTO_H_ */
