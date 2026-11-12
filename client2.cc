#include <ControlProtocol.h>
#include <TcpReader.h>
#include <UdpWriter.h>
#include <Utils.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <optional>
#include <thread>

#define SAMPLE_RATE_MS 5
#define PRINT_RATE_MS 20

int main() {
	UdpWriter writer("127.0.0.1", 4000);

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

	const auto sample_interval = std::chrono::milliseconds(SAMPLE_RATE_MS);
	const auto print_interval = std::chrono::milliseconds(PRINT_RATE_MS);

	auto sample_time = steady_start + sample_interval;
	auto print_time = steady_start + print_interval;

	float out3_old_value = 0.0f;

	// Infinite work loop
	while (1) {
		auto now = std::chrono::steady_clock::now();
		if (now >= sample_time) {
			sample_time = sample_time + sample_interval;

			readValue(reader1, out1);
			readValue(reader2, out2);
			readValue(reader3, out3);
		}

		if (now >= print_time) {
			print_time = print_time + print_interval;

			auto elapsed = now - steady_start;
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

		// Task 2: Control output 1 based on value of output 3
		float out3_value;
		if (Utils::ParseFloat(out3, out3_value)) {
			std::optional<uint16_t> frequency;
			std::optional<uint16_t> amplitude;

			// When the value on the output 3 of the server becomes greater than or equal 3.0:
			// - Set the frequency of server output 1 to 1Hz.
			// - Set the amplitude of server output 1 to 8000.
			if (out3_old_value < 3.0f && out3_value >= 3.0f) {
				frequency = 1000;
				amplitude = 8000;
			}
			// When the value on the output 3 becomes lower than 3.0:
			// - Set the frequency of server output 1 to 2Hz.
			// - Set the amplitude of server output 1 to 4000.
			else if (out3_old_value >= 3.0f && out3_value < 3.0f) {
				frequency = 2000;
				amplitude = 4000;
			}
			out3_old_value = out3_value;

			if (frequency.has_value()) {
				ControlMessage message(MessageOperation_Write, MessageObject_Out1, MessageProperty_Frequency, frequency.value());
				writer.Write(reinterpret_cast<std::byte*>(&message), message.Size());
			}
			if (amplitude.has_value()) {
				ControlMessage message(MessageOperation_Write, MessageObject_Out1, MessageProperty_Amplitude, amplitude.value());
				writer.Write(reinterpret_cast<std::byte*>(&message), message.Size());
			}
		}

		auto wakeup = std::min(sample_time, print_time);
		std::this_thread::sleep_until(wakeup);
	}

	return 0;
}
