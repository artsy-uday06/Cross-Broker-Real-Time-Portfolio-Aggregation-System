#ifndef RECORDPARSER_H
#define RECORDPARSER_H
#include <string>
#include <vector>
#include "Trade.h"

class RecordParser {
public:
    static std::vector<std::string> split(const std::string& line, char delim = '|');
    static std::string recordType(const std::string& line);   // "TRADE", "PRICE", "END", ...

    // Returns a heap-allocated Trade (caller/owner must delete), or nullptr if malformed.
    static Trade* parseTrade(const std::string& line);
    static bool   parsePrice(const std::string& line, std::string& ticker, double& price);
};
#endif
