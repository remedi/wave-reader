#pragma once

/// @brief A minimal wrapper for a socket file descriptor.
class Socket
{
public:
	/// Create a socket.
	/// @param domain communication domain (AF_INET, AF_INET6).
	/// @param type socket type (SOCK_STREAM, SOCK_DGRAM).
	/// @param protocol protocol to be used with the socket.
	/// @throw std::runtime_error if creating the socket fails.
	Socket(int domain, int type, int protocol);

	/// Close the socket.
	~Socket();

	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket(Socket&&) = delete;
	Socket& operator=(Socket&&) = delete;

protected:
	int fd_;
};
