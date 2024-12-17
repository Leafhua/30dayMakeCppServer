/**
 * @file server.__cpp_guaranteed_copy_elision
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Server.h"

#include "EventLoop.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  loop->Loop();
  delete server;
  delete loop;
  return 0;
}
