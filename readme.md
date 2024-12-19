# 30天打造C++ Web服务器

## 项目简介

本项目基于 GitHub 仓库 [Wlgls/30daysCppWebServer](https://github.com/Wlgls/30daysCppWebServer) 进行学习和实践，旨在通过 30 天的时间逐步实现一个高性能的 C++ Web 服务器。项目涵盖了从基础的 socket 编程到高级的多线程处理和 Reactor 模式的应用，最终构建一个能够处理高并发请求的 Web 服务器。

## 目录结构

```
30dayMakeCppServer/
├── CMakeLists.txt
├── build_support/
│   ├── format.py
│   └── lint.py
├── include/
│   ├── Acceptor.h
│   ├── Buffer.h
│   ├── Channel.h
│   ├── Connection.h
│   ├── EventLoop.h
│   ├── InetAddress.h
│   ├── Server.h
│   ├── Socket.h
│   ├── ThreadPool.h
│   └── utils.h
├── src/
│   ├── Acceptor.cpp
│   ├── Buffer.cpp
│   ├── Channel.cpp
│   ├── Connection.cpp
│   ├── EventLoop.cpp
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Socket.cpp
│   ├── ThreadPool.cpp
│   └── utils.cpp
└── tests/
    ├── test_buffer.cpp
    ├── test_connection.cpp
    ├── test_eventloop.cpp
    └── test_server.cpp
```

## 功能模块

### Day 1: 基础 Socket 编程

- 实现简单的 TCP 服务器和客户端通信。
- 主要步骤：创建 socket、绑定地址、监听连接、读写数据。

### Day 2: 错误处理

- 封装错误处理函数，提高代码的健壮性。
- 主要文件：`utils.h` 和 `utils.cpp`。

### Day 3: 高并发处理

- 使用 `epoll` 实现高并发。
- 主要步骤：创建 epoll 实例、注册监听套接字、处理事件。

### Day 4: 代码封装

- 将之前的代码封装成类，提高代码的可复用性和可维护性。
- 主要类：`Epoll`、`Socket`、`InetAddr`、`Socket`。

### Day 5: 文件描述符事件处理

- 创建 `Channel` 类封装文件描述符事件处理。
- 主要步骤：设置回调函数、处理读写事件。

### Day 6: 重构代码

- 添加 `EventLoop` 和 `Server` 类，实现服务器功能。
- 主要步骤：创建事件循环、处理连接和事件。

### Day 7: 抽象 Acceptor 类

- 将服务器的 socket 初始化设置抽象出 `Acceptor` 类。
- 主要步骤：创建 socket、绑定地址、设置监听。

### Day 8: 添加连接处理

- 新增 `Connection` 类处理连接操作（消息回复）。
- 主要步骤：读取数据、处理数据、写回数据。

### Day 9: 添加缓冲池

- 实现 `Buffer` 类，用于缓冲 socket 数据。
- 主要步骤：读取数据、存储数据、写回数据。

### Day 10: 添加线程池

- 实现多线程处理，提高并发能力。
- 主要步骤：初始化线程池、提交任务、分配任务、执行任务。

### Day 11: 完善线程池

- 使用 C++11 特性优化线程池。
- 主要特性：`auto` 关键字、`std::make_shared`、`std::bind`、`std::forward`、`std::future`、`std::result_of`、`std::unique_lock`、`std::mutex`、`std::condition_variable`、`emplace`。

### Day 12: 主从 Reactor 模式

- 改写 `Server` 类，添加 `SubReactor`，更新 `Connection`。
- 主要步骤：主 Reactor 负责监听连接请求，从 Reactor 负责处理已建立的连接。

### Day 13: 工程化

- 使用 CMake 构建工程，使用 Google 格式规范代码。
- 主要步骤：设置 `CMakeLists.txt`、配置 build support 脚本。

### Day 14: 重构 Connection

- 将 `echo` 操作拆分出来，支持业务逻辑自定义。
- 主要步骤：设置连接建立回调、处理读写事件。

## 依赖项

- C++11 或更高版本
- CMake 3.10 或更高版本
- Google Test（可选，用于单元测试）

## 编译与运行

### 编译

1. 克隆项目仓库：

   ```sh
   git clone https://github.com/yourusername/30dayMakeCppServer.git
   cd 30dayMakeCppServer
   ```

2. 创建构建目录并编译：

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

### 运行

1. 运行服务器：

   ```sh
   ./src/server
   ```

2. 运行客户端（示例）：

   ```sh
   ./src/client
   ```

## 单元测试

1. 编译测试用例：

   ```sh
   cd build
   cmake .. -DENABLE_TESTS=ON
   make
   ```

2. 运行测试用例：

   ```sh
   ./tests/test_buffer
   ./tests/test_connection
   ./tests/test_eventloop
   ./tests/test_server
   ```

## 项目贡献

欢迎贡献代码和提出改进建议！请遵循以下步骤：

1. Fork 项目仓库。
2. 创建一个新的分支：`git checkout -b feature/new-feature`。
3. 提交您的更改：`git commit -am 'Add some feature'`。
4. 推送您的分支：`git push origin feature/new-feature`。
5. 提交 Pull Request。

