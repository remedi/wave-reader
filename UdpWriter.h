#pragma once

#include <Socket.h>

#include <stdint.h>

#include <cstddef>
#include <string>

/// @brief A writer for sending datagrams to UDP sockets.
class UdpWriter : Socket
{
public:
	/// Create a UdpWriter.
	/// @param address IPv4 address.
	/// @param port UDP port.
	/// @throw std::invalid_argument if the IP address format is invalid.
	/// @throw std::runtime_error if initializing the writer fails.
	UdpWriter(const std::string& address, uint16_t port);
	~UdpWriter() = default;

	/// Send N bytes to the writer from a buffer. This call is non-blocking.
	/// @param data Pointer to a buffer from where the data is sent.
	/// @param data_size Size of the data.
	/// @return Bytes sent or 0 if call would be blocked or no data was sent.
	size_t Write(const std::byte* data, size_t data_length);

	UdpWriter(const UdpWriter&) = delete;
	UdpWriter& operator=(const UdpWriter&) = delete;
	UdpWriter(UdpWriter&&) = delete;
	UdpWriter& operator=(UdpWriter&&) = delete;
};
