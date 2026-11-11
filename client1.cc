#include <iostream>

#include <TcpReader.h>

#include <chrono>
#include <cstddef>
#include <string_view>
#include <thread>

#define SAMPLE_RATE_MS 10
#define PRINT_RATE_MS 100

int main() {
	TcpReader reader1("127.0.0.1", 4001);
	TcpReader reader2("127.0.0.1", 4002);
	TcpReader reader3("127.0.0.1", 4003);

	std::string out1 = "--";
	std::string out2 = "--";
	std::string out3 = "--";

	auto readValue = [](TcpReader& reader, std::string& out) {
		std::byte buffer[32];

		size_t bytes = reader.Read(buffer, sizeof(buffer));
		if (bytes != 0) {
			std::string data(reinterpret_cast<const char*>(buffer), bytes);

			// Process data line by line
			size_t pos;
			while ((pos = data.find('\n')) != std::string::npos) {
				out = data.substr(0, pos);
				data.erase(0, pos + 1);
			}
		}
	};

	// Timing
	const auto steady_start = std::chrono::steady_clock::now();
	const auto sys_start = std::chrono::system_clock::now();
	const auto interval = std::chrono::milliseconds(PRINT_RATE_MS);
	auto activation = steady_start + interval;

	// Infinite work loop
	while (1) {
		readValue(reader1, out1);
		readValue(reader2, out2);
		readValue(reader3, out3);

		auto now = std::chrono::steady_clock::now();
		if (now > activation) {
			activation = activation + interval; // Advance and prevent drift

			auto elapsed = activation - steady_start;
			auto timestamp = (sys_start + elapsed).time_since_epoch();

			// Adding a JSON library for outputting this is overkill,
			// we can just format the values appropriately.
			// {"timestamp": 1709286246830, "out1": "-4.8", "out2": "8.0", "out3": "--"}
			std::cout << "{\"timestamp\": " << std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count()
				<< ", \"out1\": \"" << out1
				<< "\", \"out2\": \"" << out2
				<< "\", \"out3\": \"" << out3
				<< "\"}" << std::endl;

			// Reset values
			out1 = "--";
			out2 = "--";
			out3 = "--";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_RATE_MS));
	}

	return 0;
}
