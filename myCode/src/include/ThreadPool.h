/**
 * @file ThreadPool.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <atomic>
#include <condition_variable>  // NOLINT
#include <functional>
#include <future>  // NOLINT
#include <memory>
#include <mutex>  // NOLINT
#include <queue>
#include <stdexcept>
#include <thread>  // NOLINT
#include <type_traits>
#include <utility>
#include <vector>
#include "common.h"
class ThreadPool {
 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_variable_;
  std::atomic<bool> stop_{false};

 public:
  DISALLOW_COPY_AND_MOVE(ThreadPool);
  explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
  ~ThreadPool();

  // void add(std::function<void()>);
  template <class F, class... Args>
  auto Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

// 不能放在cpp文件，原因是C++编译器不支持模版的分离编译
template <class F, class... Args>
auto ThreadPool::Add(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task =
      std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_variable_.notify_one();
  return res;
}
