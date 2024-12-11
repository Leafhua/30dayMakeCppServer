# 30dayMakeCppServer

## 简介

根据 github [Wlgls/30daysCppWebServer](https://github.com/Wlgls/30daysCppWebServer) 学习并记录实现过程

## day1

简单实现 socket，通过设置 server 与 client 实现简单通信

-   socket
    通过使用<sys/socket.h>实现 socket

    ```cpp
    //server端
    // 1. 创建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // 2. 创建serverSocket地址
    struct socketaddr_in server_addr;
    // 清空地址内容
    bzero(&server_addr, sizeof(server_addr));
    // 设置地址内容
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8883);
    // 3. 绑定socket地址
    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    // 设置client地址
    struct sockaddr_in client_addr;
    // 4. 监听并读取数据
    listen(server_socket, SOMAXCONN);
    read(accpet(server_socket, (sockaddr*)&client_addr, sizeof(client_addr)));

    char buf[1024];
    bzero(buf, sizeof(buf));
    // 5. 写入数据
    printf("message from client: %s\n", write(clnet_socket, buf, sizeof(buf)));

    ```

    server 端主要为 创建 socket->绑定 socket 地址->监听并读取数据->写入数据.

    ```cpp
    //client端
    // 1. 创建socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    // 2. 设置client地址
    client_addr.sin_family = AF_INET;
    client_addr.sin_ddr.s_addr = inet_addr("127.0.0.1");
    client_addr.sin_port = htons(8883);
    // 3. 连接server端
    connect(client_socket, (sockaddr*)&client_addr, sizeof(client_addr));

    char buf[1024];
    bzero(buf, sizeof(buf));
    // 4. 写入数据
    scanf("%s", buf);
    write(client_socket, buf, sizeof(buf));

    // 5. 读取数据
    read(client_socket, buf, sizeof(buf));
    printf("message from server: %s\n", buf);

    // 6. 关闭socket
    close(client_socket);

    ```
    client 端主要为 创建 socket->设置 client 地址->连接 server 端->写入数据->读取数据->关闭 socket.

## day2
实现错误处理
编写utils.h和utils.cpp，将错误处理封装为函数
```cpp
//utils.h
void error(bool status, const char* errmsg);

```

```cpp
//utils.cpp
void error(bool status, const char* errmsg) {
    // 打印错误信息
    perror(errmsg);
    // 退出程序
    exit(EXIT_FAILURE);
}

```

## day3
实现高并发
通过使用epoll实现高并发

> 使用通义灵码生成uml图来描述这一过程
```mermaid


graph TD
    A[创建服务器套接字] --> B[绑定服务器地址]
    B --> C[监听连接请求]
    C --> D[创建epoll实例]
    D --> E[注册监听套接字到epoll实例]
    E --> F[进入主循环]
    F --> G[等待epoll事件]
    G --> H{事件类型判断}
    H -->|监听套接字| I[接受新连接]
    I --> J[设置新连接为非阻塞]
    J --> K[注册新连接到epoll实例]
    K --> L[继续等待epoll事件]
    H -->|客户端套接字| M[读取数据]
    M --> N{读取结果判断}
    N -->|数据可读| O[处理数据]
    O --> P[写回数据]
    P --> L
    N -->|连接关闭| Q[关闭套接字]
    Q --> R[从epoll实例中删除套接字]
    R --> L
    N -->|读取错误| S[处理错误]
    S --> T[关闭套接字]
    T --> R
    R --> L
    L --> F
```
时序图
```mermaid


sequenceDiagram
    participant Server
    participant Client
    participant Epoll

    Server->>Epoll: 创建epoll实例
    Server->>Epoll: 注册监听套接字
    loop 主循环
        Epoll->>Server: 等待epoll事件
        alt 监听套接字事件
            Client->>Server: 连接请求
            Server->>Epoll: 接受新连接
            Server->>Epoll: 设置新连接为非阻塞
            Server->>Epoll: 注册新连接
        else 客户端套接字事件
            Epoll->>Server: 读取数据
            alt 数据可读
                Server->>Server: 处理数据
                Server->>Client: 写回数据
            else 连接关闭
                Server->>Epoll: 关闭套接字
                Server->>Epoll: 删除套接字
            else 读取错误
                Server->>Epoll: 处理错误
                Server->>Epoll: 关闭套接字
                Server->>Epoll: 删除套接字
            end
        end
    end

```
甘特图
```mermaid
gantt
    title epoll 高并发处理时间线
    dateFormat  YYYY-MM-DD HH:mm:ss
    section 服务器操作
    创建服务器套接字         :a1, 2023-10-01 00:00:00, 1s
    绑定服务器地址           :a2, after a1, 1s
    监听连接请求             :a3, after a2, 1s
    创建epoll实例            :a4, after a3, 1s
    注册监听套接字到epoll实例 :a5, after a4, 1s
    进入主循环               :a6, after a5, 1s
    等待epoll事件            :a7, after a6, 1s
    接受新连接               :a8, after a7, 1s
    设置新连接为非阻塞       :a9, after a8, 1s
    注册新连接到epoll实例    :a10, after a9, 1s
    读取数据                 :a11, after a10, 1s
    处理数据                 :a12, after a11, 1s
    写回数据                 :a13, after a12, 1s
    关闭套接字               :a14, after a13, 1s
    从epoll实例中删除套接字  :a15, after a14, 1s

```

## day4
将之前的代码封装成类，分别为Epoll,Socket,InetAddr,Socket.

-  **bug**
    
    - 在wsl2上使用clangd的进行编译时出现一个问题，无法正确引入cpp头文件
        wsl环境为
        ```
        Distributor ID: Ubuntu
        Description:    Ubuntu 22.04.5 LTS
        Release:        22.04
        Codename:       jammy
        ```
        在安装g++12后解决
        ```
        sudo apt install g++-12
        ```
        <!-- 参考[clang++ cannot find iostream](https://askubuntu.com/questions/1449769/clang-cannot-find-iostream) -->
    - 设置套接字非阻塞模式出错
        ```cpp
        //正确设置
        void Socket::setnonblocking() {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
        }
        
        //错误设置
        void Socket::setnonblocking() {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL | O_NONBLOCK));
        }
        ```


## day5
创建Channel类封装文件描述符事件处理。

>加入Channel后处理流程
```mermaid
flowchart TD
    A[开始] --> B[创建服务器套接字]
    B --> C[绑定地址并监听]
    C --> D[创建 Epoll 对象]
    D --> E[设置服务器套接字为非阻塞]
    E --> F[创建服务器 Channel 对象]
    F --> G[启用服务器 Channel 的读事件]
    G --> H[主循环]
    H --> I{Epoll 等待事件}
    I -->|有事件| J[获取活跃 Channel 列表]
    J --> K{遍历活跃 Channel 列表}
    K -->|是服务器套接字| L[处理新连接]
    K -->|是客户端套接字| M[处理读事件]
    K -->|其他事件| N[处理其他事件]
    L --> O[创建客户端套接字]
    O --> P[设置客户端套接字为非阻塞]
    P --> Q[创建客户端 Channel 对象]
    Q --> R[启用客户端 Channel 的读事件]
    R --> S[返回主循环]
    M --> S[返回主循环]
    N --> S[返回主循环]
    S --> H
    I -->|无事件| T[继续等待]
    T --> H
```