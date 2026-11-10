#include <iostream>

#include <TcpReader.h>

#include <chrono>
#include <cstddef>
#include <string_view>
#include <thread>

int main() {
	TcpReader out1("127.0.0.1", 4001);
	TcpReader out2("127.0.0.1", 4002);
	TcpReader out3("127.0.0.1", 4003);

	std::byte buffer[16];
	size_t bytes = 0;

	// Infinite work loop
	while (1) {
		bytes = out1.Read(buffer, sizeof(buffer));
		std::cout << std::string_view(reinterpret_cast<const char*>(buffer), bytes);

		bytes = out2.Read(buffer, sizeof(buffer));
		std::cout << std::string_view(reinterpret_cast<const char*>(buffer), bytes);

		bytes = out3.Read(buffer, sizeof(buffer));
		std::cout << std::string_view(reinterpret_cast<const char*>(buffer), bytes);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
