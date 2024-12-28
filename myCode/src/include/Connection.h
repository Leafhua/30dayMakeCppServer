/**
 * @file Connection.h
 * @author penghua zhu (zphzhupenghua@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-12-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include "common.h"

class Connection {
 public:
  enum State {
    Invalid = 1,
    Hadnshaking,
    Connected,
    Closed,
    Failed,
  };
  DISALLOW_COPY_AND_MOVE(Connection);
  Connection(int _fd, EventLoop *_loop);
  ~Connection();

  void SetOnConnect(std::function<void(Connection *)> const &_fn);
  void SetDeleteConnection(std::function<void(int)> const &_fn);
  void SetOnRecv(std::function<void(Connection *)> const &_fn);

  [[nodiscard]] State GetState() const;
  [[nodiscard]] Socket *GetSocket() const;
  void SetSendBuf(const char *_str);
  Buffer *ReadBuf();
  Buffer *SendBuf();

  RC Read();
  RC Write();
  RC Send(const std::string &msg);

  void Close();

  void OnConnect(std::function<void()> _fn);
  void OnMessage(std::function<void()> _fn);

 private:
  std::unique_ptr<Channel> channel_;
  std::unique_ptr<Socket> socket_;

  enum State state_;
  std::unique_ptr<Buffer> read_buf_;
  std::unique_ptr<Buffer> send_buf_;

  std::function<void(int)> delete_connection_;
  std::function<void(Connection *)> on_recv_;

  RC ReadNonBlocking();
  RC WriteNonBlocking();
  RC ReadBlocking();
  RC WriteBlocking();
  void Business();
};
