// main.cpp  -  Owner: Uday Raj Singh
//   ./portfolio                 wait for the Python process on TCP port 5050
//   ./portfolio --file FILE     read records from FILE (test the parser without sockets)
#include <iostream>
#include <fstream>
#include <string>
#include "Portfolio.h"
#include "RecordParser.h"
#include "TcpReceiver.h"

static void handleRecord(const std::string& line, Portfolio& pf, int& bad) {
    std::string type = RecordParser::recordType(line);
    if (type == "TRADE") {
        Trade* t = RecordParser::parseTrade(line);
        if (t) pf.addTrade(t); else { ++bad; std::cerr << "bad record: " << line << "\n"; }
    } else if (type == "PRICE") {
        std::string ticker; double price = 0;
        if (RecordParser::parsePrice(line, ticker, price)) pf.updatePrice(ticker, price);
        else { ++bad; std::cerr << "bad record: " << line << "\n"; }
    } else if (type != "END" && !type.empty()) {
        ++bad; std::cerr << "unknown record: " << line << "\n";
    }
}

int main(int argc, char* argv[]) {
    Portfolio pf;
    int received = 0, bad = 0;
    std::string line;

    if (argc == 3 && std::string(argv[1]) == "--file") {
        std::ifstream in(argv[2]);
        if (!in) { std::cerr << "cannot open " << argv[2] << "\n"; return 1; }
        while (std::getline(in, line)) {
            if (line == "END") break;
            ++received; handleRecord(line, pf, bad);
        }
    } else {
        TcpReceiver rx;
        if (!rx.start(5050) || !rx.acceptClient()) return 1;
        while (rx.readLine(line)) {
            if (line == "END") break;
            ++received; handleRecord(line, pf, bad);
        }
    }

    std::cout << "[C++] records received: " << received << ", rejected: " << bad << "\n";
    pf.printHoldings();
    pf.printBrokerSummary();
    pf.printTradeHistory();
    return 0;        // Portfolio destructor frees all trades and holdings
}
