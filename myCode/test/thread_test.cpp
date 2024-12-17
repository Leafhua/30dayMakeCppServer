#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include "ThreadPool.h"
void Print(int a, double b, const char *c, std::string const &d) { std::cout << a << b << c << d << std::endl; }

void Test() { std::cout << "help" << std::endl; }

int main() {
  ThreadPool *pool = new ThreadPool();
  std::function<void()> func = std::bind(Print, 1, 3.14, "hello", std::string("world"));
  pool->Add(func);
  func = Test;
  pool->Add(func);
  delete pool;
  return 0;
}
