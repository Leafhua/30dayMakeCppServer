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

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
class ThreadPool {

private:
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;
  std::mutex tasks_mtx;
  std::condition_variable cv;

  bool stop;

public:
  ThreadPool(int size = std::thread::hardware_concurrency());
  // ThreadPool(int size = 10);
  ~ThreadPool();

  // void add(std::function<void()>);
  template <class F, class... Args>
  auto add(F &&f, Args &&...args)
      -> std::future<typename std::result_of<F(Args...)>::type>;
};


//不能放在cpp文件，原因是C++编译器不支持模版的分离编译
template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);

    if (stop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.emplace([task]() {(*task)();});

  }
  cv.notify_one();
  return res;

}
