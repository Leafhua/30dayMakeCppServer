/**
 * @file ThreadPool.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ThreadPool.h"
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

ThreadPool::ThreadPool(int size) : stop(false) {
  for (int i = 0; i < size; ++i) {
    threads.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(tasks_mtx);
          cv.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty()) {
            return;
          }
          task = tasks.front();
          tasks.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    stop = true;
  }
  cv.notify_all();
  for (std::thread &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
}


