#pragma once

class Socket
{
public:
	Socket(int domain, int type, int protocol);
	~Socket();

	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket(Socket&&) = delete;
	Socket& operator=(Socket&&) = delete;

protected:
	int fd_;
};
