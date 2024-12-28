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
#include <cstddef>
#include <string>
#include "common.h"
class Buffer {
 public:
  DISALLOW_COPY_AND_MOVE(Buffer);
  Buffer() = default;
  ~Buffer() = default;

  [[nodiscard]] const std::string &Buf() const;
  [[nodiscard]] const char *CStr() const;
  void SetBuf(const char *_buf);

  [[nodiscard]] size_t Size() const;
  void Append(const char *_str, int _size);
  void Clear();

 private:
  std::string buf_;
};
