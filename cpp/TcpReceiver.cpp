
#include "TcpReceiver.h"
#include <iostream>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #define CLOSESOCK closesocket
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define CLOSESOCK close
#endif

TcpReceiver::TcpReceiver() : listenSock_(-1), clientSock_(-1) {
#ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

TcpReceiver::~TcpReceiver() {
    if (clientSock_ != -1) CLOSESOCK(clientSock_);
    if (listenSock_ != -1) CLOSESOCK(listenSock_);
#ifdef _WIN32
    WSACleanup();
#endif
}

bool TcpReceiver::start(int port) {
    listenSock_ = (long)socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock_ < 0) { std::cerr << "socket() failed\n"; return false; }

    int yes = 1;
    setsockopt(listenSock_, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    for (unsigned i = 0; i < sizeof(addr.sin_zero); ++i) addr.sin_zero[i] = 0;

    if (bind(listenSock_, (sockaddr*)&addr, sizeof(addr)) < 0) { std::cerr << "bind() failed\n"; return false; }
    if (listen(listenSock_, 1) < 0) { std::cerr << "listen() failed\n"; return false; }
    std::cout << "[C++] listening on 127.0.0.1:" << port << std::endl;
    return true;
}

bool TcpReceiver::acceptClient() {
    clientSock_ = (long)accept(listenSock_, NULL, NULL);
    if (clientSock_ < 0) { std::cerr << "accept() failed\n"; return false; }
    std::cout << "[C++] Python process connected" << std::endl;
    return true;
}

bool TcpReceiver::readLine(std::string& line) {
    while (true) {
        size_t pos = buffer_.find('\n');
        if (pos != std::string::npos) {
            line = buffer_.substr(0, pos);
            buffer_.erase(0, pos + 1);
            if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
            return true;
        }
        char chunk[1024];
        int n = (int)recv(clientSock_, chunk, sizeof(chunk), 0);
        if (n <= 0) {                       // connection closed
            if (buffer_.empty()) return false;
            line = buffer_; buffer_.clear(); return true;
        }
        buffer_.append(chunk, n);
    }
}
