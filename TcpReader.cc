#include <TcpReader.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdexcept>

TcpReader::TcpReader(const std::string& address, uint16_t port) : Socket(AF_INET, SOCK_STREAM, 0) {
	struct sockaddr_in sa = {};
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	int ret = inet_pton(AF_INET, address.c_str(), &sa.sin_addr);
	if (ret <= 0) {
		throw std::runtime_error("Invalid address");
	}

	ret = connect(fd_, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa));
	if (ret != 0) {
		throw std::runtime_error("Failed to connect");
	}
}

size_t TcpReader::Read(std::byte* buffer, size_t buffer_size) {
	ssize_t bytes = recv(fd_, buffer, buffer_size, 0);
	return static_cast<size_t>(bytes); // TODO
}
