#ifndef UART_HPP_
#define UART_HPP_

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

class UART {
public:
  UART();
  ~UART();

  bool init(const char* device, speed_t baud_rate);
  ssize_t send(const void* data, size_t size);
  ssize_t receive(void* data, size_t size, int timeout_ms);

private:
  int uart_fd_;
};

#endif // UART_HPP_