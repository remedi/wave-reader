#include <UdpWriter.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdexcept>

UdpWriter::UdpWriter(const std::string& address, uint16_t port) : Socket(AF_INET, SOCK_DGRAM, 0) {
	struct sockaddr_in sa = {};
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	int ret = inet_pton(AF_INET, address.c_str(), &sa.sin_addr);
	if (ret <= 0) {
		throw std::invalid_argument("Invalid address");
	}

	// Socket is not connected, but address is stored for sending
	ret = connect(fd_, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa));
	if (ret != 0) {
		throw std::runtime_error("Failed to connect");
	}
}

size_t UdpWriter::Write(const std::byte* data, size_t data_size) {
	ssize_t bytes = send(fd_, data, data_size, MSG_DONTWAIT);
	if (bytes <= 0) { // EAGAIN, EWOULDBLOCK
		return 0;
	}
	return static_cast<size_t>(bytes);
}
