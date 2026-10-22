
#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H
#include <string>

class TcpReceiver {
private:
    long        listenSock_;
    long        clientSock_;
    std::string buffer_;
public:
    TcpReceiver();
    ~TcpReceiver();
    bool start(int port);       
    bool acceptClient();                  
    bool readLine(std::string& line);     
};
#endif
