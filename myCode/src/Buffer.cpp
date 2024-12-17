/**
 * @file Buffer.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Buffer.h"
#include <sys/types.h>
#include <iostream>
#include <string>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::Append(const char *_str, int _size) {
  for (int i = 0; i < _size; ++i) {
    if (_str[i] == '\0') {
      break;
    }
    buf_.push_back(_str[i]);
  }
}

ssize_t Buffer::Size() { return buf_.size(); }

const char *Buffer::ToStr() { return buf_.c_str(); }

void Buffer::Clear() { buf_.clear(); }

void Buffer::Getline() {
  buf_.clear();
  std::getline(std::cin, buf_);
}

void Buffer::SetBuf(const char *_buf) {
  buf_.clear();
  buf_.append(_buf);
}
