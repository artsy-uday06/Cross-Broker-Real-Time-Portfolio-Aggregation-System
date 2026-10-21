#include "RecordParser.h"
#include <sstream>
#include <stdexcept>

std::vector<std::string> RecordParser::split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) parts.push_back(item);
    return parts;
}

std::string RecordParser::recordType(const std::string& line) {
    std::vector<std::string> p = split(line);
    return p.empty() ? "" : p[0];
}

Trade* RecordParser::parseTrade(const std::string& line) {
    std::vector<std::string> p = split(line);
    if (p.size() != 6 || p[0] != "TRADE") return nullptr;
    try {
        int    qty   = std::stoi(p[3]);
        double price = std::stod(p[4]);
        return new Trade(p[1], p[2], qty, price, p[5]);
    } catch (const std::exception&) {
        return nullptr;
    }
}

bool RecordParser::parsePrice(const std::string& line, std::string& ticker, double& price) {
    std::vector<std::string> p = split(line);
    if (p.size() != 3 || p[0] != "PRICE") return false;
    try {
        ticker = p[1];
        price  = std::stod(p[2]);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}
