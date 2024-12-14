/**
 * @file Buffer.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <string>
#include <sys/types.h>
class Buffer {

private:
  std::string buf;

public:
  Buffer();
  ~Buffer();

  ssize_t size();

  const char *c_str();

  void append(const char *_str, int _size);
  void clear();
  void getline();
};
