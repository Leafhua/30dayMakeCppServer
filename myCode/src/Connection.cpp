/**
 * @file Connection.cpp
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Connection.h"

#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "common.h"

#define READ_BUFFER 1024

// cpp成员变量声明顺序注意
// 可能导致的问题：如果一个成员变量的初始化依赖于另一个成员变量的值，必须确保依赖的变量在前面声明并初始化。否则，可能会导致未定义行为。
Connection::Connection(int _fd, EventLoop *_loop) {
  socket_ = std::make_unique<Socket>();
  socket_->SetFd(_fd);
  if (_loop != nullptr) {
    channel_ = std::make_unique<Channel>(_fd, _loop);
    channel_->EnableRead();
    channel_->EnableET();
  }
  read_buf_ = std::make_unique<Buffer>();
  send_buf_ = std::make_unique<Buffer>();
  state_ = State::Connected;
}

Connection::~Connection() = default;

RC Connection::Read() {
  if (state_ != State::Connected) {
    perror("Connection is not connected, can not read");
    return RC_CONNECTION_ERROR;
  }
  assert(state_ == State::Connected && "connection state is disconnected!");
  read_buf_->Clear();
  if (socket_->IsNonBlocking()) {
    return ReadNonBlocking();
  }
  return ReadBlocking();
}

RC Connection::Write() {
  if (state_ != State::Connected) {
    perror("Connection is not connected, can not write");
    return RC_CONNECTION_ERROR;
  }
  RC rc_temp = RC_UNDEFINED;
  if (socket_->IsNonBlocking()) {
    rc_temp = WriteNonBlocking();
  } else {
    rc_temp = WriteBlocking();
  }
  send_buf_->Clear();
  return rc_temp;
}

RC Connection::ReadNonBlocking() {
  int sockfd = socket_->Fd();
  char buf[1024];
  while (true) {
    memset(buf, 0, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      // 检查 bytes_read 是否在 int 的范围内
      if (bytes_read > INT16_MAX) {
        printf("Error: bytes_read exceeds int range\n");
        state_ = State::Closed;
        Close();
        break;
      }
      read_buf_->Append(buf, static_cast<int>(bytes_read));
    } else if (bytes_read == -1 && errno == EINTR) {
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
      break;
    } else if (bytes_read == 0) {
      printf("read EOF, client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
      Close();
      break;
    } else {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      Close();
      break;
    }
  }
  return RC_SUCCESS;
}

RC Connection::WriteNonBlocking() {
  int sockfd = socket_->Fd();
  char buf[send_buf_->Size()];
  memcpy(buf, send_buf_->CStr(), send_buf_->Size());
  size_t data_size = send_buf_->Size();
  size_t data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
  return RC_SUCCESS;
}

RC Connection::ReadBlocking() {
  int sockfd = socket_->Fd();
  // unsigned int rcv_size = 0;
  // socklen_t len = sizeof(rcv_size);
  // getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  size_t data_size = socket_->RecvBufSize();
  char buf[1024];
  ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    // 不是很安全，未检查大小
    read_buf_->Append(buf, static_cast<int>(bytes_read));
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state_ = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
  return RC_SUCCESS;
}

/**
 * @brief Never used by server, only for client
 *
 */
RC Connection::WriteBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = socket_->Fd();
  ssize_t bytes_write = write(sockfd, send_buf_->CStr(), send_buf_->Size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
  return RC_SUCCESS;
}

/**
 * @brief 参数 msg
 * 做了简单修改
 * 在每次调用时都被复制，但实际上它只作为常量引用使用。
 * 这会导致不必要的性能开销，特别是在处理大字符串时。
 * 建议将参数改为常量引用以避免不必要的拷贝。
 *
 * @param msg
 * @return RC
 */
RC Connection::Send(const std::string &msg) {
  SetSendBuf(msg.c_str());
  Write();
  return RC_SUCCESS;
}

void Connection::Business() {
  Read();
  on_recv_(this);
}

void Connection::Close() { delete_connection_(socket_->Fd()); }

void Connection::SetDeleteConnection(std::function<void(int)> const &_fn) { delete_connection_ = _fn; }

void Connection::SetOnRecv(const std::function<void(Connection *)> &_fn) {
  on_recv_ = _fn;
  std::function<void()> bus = std::bind(&Connection::Business, this);
  channel_->SetReadCallback(bus);
}

Connection::State Connection::GetState() const { return state_; }

Socket *Connection::GetSocket() const { return socket_.get(); }

void Connection::SetSendBuf(const char *_str) { send_buf_->SetBuf(_str); }

Buffer *Connection::ReadBuf() { return read_buf_.get(); }

Buffer *Connection::SendBuf() { return send_buf_.get(); }
