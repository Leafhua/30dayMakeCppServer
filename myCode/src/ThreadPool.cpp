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

ThreadPool::ThreadPool(unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    workers_.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_variable_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) {
            return;
          }
          task = tasks_.front();
          tasks_.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_variable_.notify_all();
  for (std::thread &worker_th : workers_) {
    if (worker_th.joinable()) {
      worker_th.join();
    }
  }
}
