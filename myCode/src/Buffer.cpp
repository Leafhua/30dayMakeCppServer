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

// TODO(hua): 未声明问题，目前不知道为什么tidy会报错


const std::string &Buffer::Buf() const { return buf_; }

const char *Buffer::CStr() const { return buf_.c_str(); }

size_t Buffer::Size() const { return buf_.size(); }

void Buffer::SetBuf(const char *_buf) {
  std::string new_buf(_buf);
  buf_.swap(new_buf);
}

void Buffer::Append(const char *_str, int _size) {
  for (int i = 0; i < _size; ++i) {
    if (_str[i] == '\0') {
      break;
    }
    buf_.push_back(_str[i]);
  }
}

void Buffer::Clear() { buf_.clear(); }
