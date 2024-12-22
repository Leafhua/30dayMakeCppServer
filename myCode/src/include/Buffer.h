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

#include <sys/types.h>
#include <string>
#include "Macros.h"
class Buffer {
 public:
  Buffer() = default;
  ~Buffer() = default;

  DISALLOW_COPY_AND_MOVE(Buffer);

  ssize_t Size();

  const char *ToStr();

  void Append(const char *_str, int _size);
  void Clear();
  void Getline();
  void SetBuf(const char *);

 private:
  std::string buf_;
};
