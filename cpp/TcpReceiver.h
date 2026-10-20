// TcpReceiver.h  -  Owner: Uday Raj Singh
// Minimal local TCP server: listens on 127.0.0.1, accepts ONE client (the Python
// process) and hands back its records line by line.  STATUS: in progress / needs testing.
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
    bool start(int port);                 // bind + listen
    bool acceptClient();                  // blocks until Python connects
    bool readLine(std::string& line);     // false when the client closes the connection
};
#endif
