#include "geist_protocol/nanopb/nanopb.hpp"
#include "pb_encode.h"
#include "pb_decode.h"
#include <cstdio>
#include <cstdint>

namespace NanoPB
{

  // Deserialize a SimpleMessage from a buffer
  bool deserialize_simple_message(const std::vector<uint8_t> &buffer, SimpleMessage &message)
  {
    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_decode(&stream, SimpleMessage_fields, &message);
    // TODO: Remove this before merge
    // print the buffer
    for (size_t i = 0; i < buffer.size(); i++)
    {
      printf("%02x ", buffer[i]);
    }
    printf("\n");

    if (!status)
    {
      printf("[deserialize_simple_message] Decoding here failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
  }

  // Serialize a SimpleMessage to a buffer
  std::vector<uint8_t> serialize_simple_message(const SimpleMessage &message)
  {
    std::vector<uint8_t> buffer(SimpleMessage_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_encode(&stream, SimpleMessage_fields, &message);

    if (!status)
    {
      // Failed to encode the message
      buffer.clear();
    }

    return buffer;
  }

  // Deserialize a RequestMessage from a buffer
  bool deserialize_request_message(const std::vector<uint8_t> &buffer, Request &message)
  {
    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_decode(&stream, Request_fields, &message);

    if (!status)
    {
      printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
  }

  // Deserialize a ResponseMessage from a buffer
  bool deserialize_response_message(const std::vector<uint8_t> &buffer, Response &message)
  {
    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_decode(&stream, Response_fields, &message);

    if (!status)
    {
      printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
  }


  // Serialize a RequestMessage to a buffer
  std::vector<uint8_t> serialize_request_message(const Request &message)
  {
    std::vector<uint8_t> buffer(Request_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_encode(&stream, Request_fields, &message);

    if (!status)
    {
      // Failed to encode the message
      buffer.clear();
      printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return buffer;
  }

  // Serialize a ResponseMessage to a buffer
  std::vector<uint8_t> serialize_response_message(const Response &message)
  {
    std::vector<uint8_t> buffer(Response_size);
    pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), buffer.size());
    bool status = pb_encode(&stream, Response_fields, &message);

    if (!status)
    {
      // Failed to encode the message
      buffer.clear();
      printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return buffer;
  }

} // namespace nanopb