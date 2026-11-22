#include <iostream>
#include <string.h>
#include "TextUtil.h"
#include "TcpReceiver.h"

#ifdef _WIN32
  #include <winsock2.h>
  #define CLOSESOCK closesocket
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define CLOSESOCK close
#endif
using namespace std;

// constructor: sets up the socket system
TcpReceiver::TcpReceiver() {
    listenSock = -1;
    clientSock = -1;
    bufLen = 0;
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

// destructor: closes the sockets
TcpReceiver::~TcpReceiver() {
    if (clientSock != -1) CLOSESOCK(clientSock);
    if (listenSock != -1) CLOSESOCK(listenSock);
#ifdef _WIN32
    WSACleanup();
#endif
}

// creates the socket and starts listening on the port
bool TcpReceiver::start(int port) {
    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) { cout << "socket failed\n"; return false; }

    int yes = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    for (unsigned i = 0; i < sizeof(addr.sin_zero); i++) addr.sin_zero[i] = 0;

    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) < 0) { cout << "bind failed\n"; return false; }
    if (listen(listenSock, 1) < 0) { cout << "listen failed\n"; return false; }
    cout << "[C++] waiting on port " << port << endl;
    return true;
}

// waits until Python connects
bool TcpReceiver::acceptClient() {
    clientSock = accept(listenSock, NULL, NULL);
    if (clientSock < 0) { cout << "accept failed\n"; return false; }
    cout << "[C++] Python connected" << endl;
    return true;
}

// reads one line of text sent by Python
bool TcpReceiver::readLine(char line[], int size) {
    while (true) {
        int pos = -1;
        for (int i = 0; i < bufLen; i++) {
            if (buffer[i] == '\n') { pos = i; break; }
        }
        if (pos != -1) {
            int len = pos;
            if (len > size - 1) len = size - 1;
            memcpy(line, buffer, len);
            line[len] = '\0';
            removeNewline(line);
            memmove(buffer, buffer + pos + 1, bufLen - pos - 1);
            bufLen = bufLen - pos - 1;
            return true;
        }
        if (bufLen == BUFFER_SIZE) bufLen = 0;

        int n = recv(clientSock, buffer + bufLen, BUFFER_SIZE - bufLen, 0);
        if (n <= 0) {
            if (bufLen == 0) return false;
            int len = bufLen;
            if (len > size - 1) len = size - 1;
            memcpy(line, buffer, len);
            line[len] = '\0';
            removeNewline(line);
            bufLen = 0;
            return true;
        }
        bufLen = bufLen + n;
    }
}

