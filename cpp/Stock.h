#ifndef STOCK_H
#define STOCK_H
#include "TextUtil.h"

const int MAX_CANDLES = 200;

struct Candle {
    char date[DATE_LEN];
    double open;
    double high;
    double low;
    double close;
};

class Stock {
public:
    char ticker[TEXT_LEN];
    char name[NAME_LEN];
    char sector[TEXT_LEN];
    double price;
    Candle candles[MAX_CANDLES];
    int candleCount;

    Stock(const char t[]);
    void addCandle(Candle c);
};
#endif
