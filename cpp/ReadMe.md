

### 源码安装向导
1. 进入源码路径
2. cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
3. 进入 build 目录 (cd build)
4. make
5. cmake --install --prefix <install-path>

### 生成文件
- lib/libchat-shared.so
- chat-client <ip> <port> 
- chat-server <port>