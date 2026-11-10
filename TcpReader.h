#pragma once

#include <Socket.h>

#include <stdint.h>

#include <cstddef>
#include <string>

/// @brief A non-blocking reader for receiving data from a TCP socket.
class TcpReader : Socket
{
public:
	/// Create and connect a TcpReader.
	/// @param address IPv4 address.
	/// @param port TCP port.
	/// @throw std::invalid_argument if the IP address format is invalid.
	/// @throw std::runtime_error if connecting or initializing the reader fails.
	TcpReader(const std::string& address, uint16_t port);
	~TcpReader() = default;

	/// Read up to N bytes from the reader to a buffer. This call is non-blocking.
	/// @param buffer Buffer to store the data.
	/// @param buffer_size Size of the buffer.
	/// @return Bytes read or 0 if call would be blocked or there is no data.
	size_t Read(std::byte* buffer, size_t buffer_size);

	TcpReader(const TcpReader&) = delete;
	TcpReader& operator=(const TcpReader&) = delete;
	TcpReader(TcpReader&&) = delete;
	TcpReader& operator=(TcpReader&&) = delete;
};
