#include "geist_protocol/uart/uart.hpp"
#include <sys/time.h>
#include <stdio.h>

UART::UART()
  : uart_fd_(-1) {}

UART::~UART()
{
  if (uart_fd_ != -1)
    close(uart_fd_);
}

bool UART::init(const char* device, speed_t baud_rate)
{
  // Open UART device and configure it
  uart_fd_ = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (uart_fd_ == -1)
    return false;

  struct termios options;
  tcgetattr(uart_fd_, &options);

  // Set baud rate, data bits, stop bits, and parity
  cfsetispeed(&options, baud_rate);
  cfsetospeed(&options, baud_rate);
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CRTSCTS;

  // Set raw input mode and disable canonical mode
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  // Disable software flow control
  options.c_iflag &= ~(IXON | IXOFF | IXANY);

  // Set the options
  if (tcsetattr(uart_fd_, TCSANOW, &options) != 0) { // Check for error
      perror("Error setting UART attributes");
      return false;
  }

  return true;
}

ssize_t UART::send(const void* data, size_t size) {
    if (uart_fd_ != -1) {
        ssize_t result = write(uart_fd_, data, size);
        if (result == -1) {
            perror("Error writing to UART");
        }
        return result;
    }
    return -1;
}

ssize_t UART::receive(void* data, size_t size, int timeout_ms)
{
  if (uart_fd_ != -1)
  {
    struct termios options;
    struct timeval timeout;
    fd_set readfds;
    int ret;

    // Set timeout value
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    // Set file descriptor for select
    FD_ZERO(&readfds);
    FD_SET(uart_fd_, &readfds);

    // Set terminal attributes
    tcgetattr(uart_fd_, &options);

    // Set non-blocking mode
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    if (timeout_ms > 0) {
      // Set timeout mode
      options.c_cc[VTIME] = timeout_ms / 100;
    }

    tcsetattr(uart_fd_, TCSANOW, &options);

    // Wait for data to be available or timeout
    ret = select(uart_fd_ + 1, &readfds, NULL, NULL, &timeout);
    if (ret < 0)
      return -1;
    else if (ret == 0)
      return 0;  // Timeout

    // Data is available, read it
    return read(uart_fd_, data, size);
  }

  return -1;
}