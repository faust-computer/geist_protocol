#ifndef NANOPB_HPP_
#define NANOPB_HPP_

#include <vector>
#include "request.pb.h"
#include "response.pb.h"
#include "simple.pb.h"


namespace NanoPB {

// Deserialize a SimpleMessage to a buffer
bool deserialize_simple_message(const std::vector<uint8_t>& buffer, SimpleMessage& message);

// Serialize a SimpleMessage to a buffer
std::vector<uint8_t> serialize_simple_message(const SimpleMessage& message);

// Serialize a RequestMessage to a buffer
std::vector<uint8_t> serialize_request_message(const Request& message);

// Serialize a ResponseMessage to a buffer
std::vector<uint8_t> serialize_response_message(const Response& message);

// Deserialize a RequestMessage from a buffer
bool deserialize_request_message(const std::vector<uint8_t>& buffer, Request& message);

// Deserialize a ResponseMessage from a buffer
bool deserialize_response_message(const std::vector<uint8_t>& buffer, Response& message);


} // namespace nanopb

#endif // NANOPB_HPP_