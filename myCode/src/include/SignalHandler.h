/**
 * @file SignalHandler.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <csignal>
#include <functional>
#include <map>
std::map<int, std::function<void()>> handlers_;

void signal_handler(int sig) { handlers_[sig](); }

struct Signal {
  static void signal(int sig, const std::function<void()> &handler) {
    handlers_[sig] = handler;
    ::signal(sig, signal_handler);
  }
};
