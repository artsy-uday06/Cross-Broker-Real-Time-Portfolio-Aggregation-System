#include <iostream>
#include <string.h>
#include "Stock.h"
using namespace std;

// constructor of Stock
Stock::Stock(const char t[]) {
    copyText(ticker, t, TEXT_LEN);
    copyText(name, t, NAME_LEN);
    copyText(sector, "-", TEXT_LEN);
    price = 0;
    candleCount = 0;
}

// adds one day candle to the stock
void Stock::addCandle(Candle c) {
    if (candleCount < MAX_CANDLES) {
        candles[candleCount] = c;
        candleCount++;
    }
}
