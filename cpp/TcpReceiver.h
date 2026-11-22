#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

const int BUFFER_SIZE = 4096;

class TcpReceiver {
public:
    long listenSock;
    long clientSock;
    char buffer[BUFFER_SIZE];
    int bufLen;

    TcpReceiver();
    ~TcpReceiver();
    bool start(int port);
    bool acceptClient();
    bool readLine(char line[], int size);
};
#endif

