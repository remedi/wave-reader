#include <Socket.h>

#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

Socket::Socket(int domain, int type, int protocol) {
	fd_ = socket(domain, type, protocol);
	if (fd_ < 0) {
		throw std::runtime_error("Failed to create a socket");
	}
}

Socket::~Socket() {
	close(fd_);
}
