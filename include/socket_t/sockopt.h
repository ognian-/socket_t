#ifndef SOCKET_T_SOCKOPT_H
#define SOCKET_T_SOCKOPT_H

SOCKT_NAMESPACE_BEGIN

template<int Level, int Name, typename Type, bool Readable, bool Writeable>
struct sockopt {
	using type = Type;
	static constexpr auto size = sizeof(type);
	static constexpr auto level = Level;
	static constexpr auto name = Name;
	static constexpr auto readable = Readable;
	static constexpr auto writeable = Writeable;
};

SOCKT_NAMESPACE_END

#endif // SOCKET_T_SOCKOPT_H
