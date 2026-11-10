#pragma once

#include <Socket.h>

#include <stdint.h>

#include <cstddef>
#include <string>

class TcpReader : Socket
{
public:
	TcpReader(const std::string& address, uint16_t port);
	~TcpReader() = default;

	/*
	* Read up to N bytes from the reader. This call is non-blocking.
	* @param buffer Buffer to store the data.
	* @param buffer_size Size of the buffer.
	* @return Bytes read or 0 if call would be blocked or there is no data.
	*/
	size_t Read(std::byte* buffer, size_t buffer_size);

	TcpReader(const TcpReader&) = delete;
	TcpReader& operator=(const TcpReader&) = delete;
	TcpReader(TcpReader&&) = delete;
	TcpReader& operator=(TcpReader&&) = delete;
};
