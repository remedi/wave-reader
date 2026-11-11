#pragma once

#include <arpa/inet.h>
#include <stdint.h>

// The control protocol adheres to the following specifications:
//
// * The protocol is binary.
// * Messages are structured into fields.
// * Each field has a width of 16 bits.
// * Field should be interpreted as unsigned integers.
// * The byte order is Big-Endian.
// * The first field in each message specifies the "operation".
//
// * Supported operations are:
//     * `1` - read
//     * `2` - write
//
// * Each read message consists of three fields: operation, object, and property.
// * Each write message consists of four fields: operation, object, property, and value.

enum MessageOperation : uint16_t {
	MessageOperation_Read = 1,
	MessageOperation_Write = 2
};

enum MessageObject : uint16_t {
	MessageObject_Out1 = 1,
	MessageObject_Out2 = 2,
	MessageObject_Out3 = 3
};

enum MessageProperty : uint16_t {
	MessageProperty_Enabled = 14,
	MessageProperty_MinDuration = 42,
	MessageProperty_MaxDuration = 43,
	MessageProperty_Amplitude = 170,
	MessageProperty_Frequency = 255,
	MessageProperty_GlitchChance = 300
};

struct __attribute__((__packed__)) ControlMessage {
	uint16_t operation_;
	uint16_t object_;
	uint16_t property_;
	uint16_t opt_value_;
	bool has_opt_value_ = false;

	/// @brief Create a ControlProtocol message (used for reading a property)
	/// @param operation Message operation
	/// @param object Target of the operation
	/// @param property Property of the object
	ControlMessage(MessageOperation operation, MessageObject object, MessageProperty property)
		: operation_(htons(operation)), object_(htons(object)), property_(htons(property)) {
	}

	/// @brief Create a ControlProtocol message with a value (used for writing a property)
	/// @param operation Message operation
	/// @param object Target of the operation
	/// @param property Property of the object
	/// @param value Value for the property
	ControlMessage(MessageOperation operation, MessageObject object, MessageProperty property, uint16_t value)
		: operation_(htons(operation)), object_(htons(object)), property_(htons(property)), opt_value_(htons(value)), has_opt_value_(true) {
	}

	size_t Size() {
		if (has_opt_value_) {
			return 4 * sizeof(uint16_t);
		}
		return 3 * sizeof(uint16_t);
	}
};
