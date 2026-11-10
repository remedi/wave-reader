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

	size_t Read(std::byte* buffer, size_t buffer_size);

	TcpReader(const TcpReader&) = delete;
	TcpReader& operator=(const TcpReader&) = delete;
	TcpReader(TcpReader&&) = delete;
	TcpReader& operator=(TcpReader&&) = delete;
};
