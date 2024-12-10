#include <fcntl.h>
#include <iostream>

int main() {
  std::cout << "hello, world!!" << std::endl;
  std::cout << (O_NONBLOCK) << std::endl;
  std::cout << (O_NONBLOCK | O_RDONLY) << std::endl;
  std::cout << (O_NONBLOCK | O_WRONLY) << std::endl;
  std::cout << (O_WRONLY | O_NONBLOCK) << std::endl;
  std::cout << (O_RDONLY | O_NONBLOCK) << std::endl;

  return 0;
}
