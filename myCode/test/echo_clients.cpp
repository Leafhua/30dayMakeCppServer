/**
 * @file multiple_client.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <unistd.h>
#include <cstring>

#include <functional>
#include <iostream>

#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

void OneClient(int msgs, int wait) {
  Socket *sock = new Socket;
  sock->Connect("127.0.0.1", 8883);
  Connection *conn = new Connection(nullptr, sock);
  sleep(wait);
  int count = 0;
  while (count < msgs) {
    conn->SetSendBuffer("I'm client!");
    conn->Write();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      break;
    }
    conn->Read();
    std::cout << "msg count " << count++ << ": " << conn->ReadBuffer() << std::endl;
  }
  delete conn;
}

int main(int argc, char *argv[]) {
  int threads = 100;
  int msgs = 100;
  int wait = 0;
  int o = -1;
  const char *optstring = "t:m:w:";
  while ((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 't':
        threads = std::stoi(optarg);
        break;
      case 'm':
        msgs = std::stoi(optarg);
        break;
      case 'w':
        wait = std::stoi(optarg);
        break;
      case '?':
        printf("error optopt: %c\n", optopt);
        printf("error opterr: %d\n", opterr);
        break;
      default:
        break;
    }
  }

  ThreadPool *pool = new ThreadPool(threads);
  std::function<void()> func = std::bind(OneClient, msgs, wait);
  for (int i = 0; i < threads; ++i) {
    pool->Add(func);
  }
  delete pool;
  return 0;
}
