#include <iostream>
#include <string.h>
#include "Trade.h"
using namespace std;

// default constructor of Trade
Trade::Trade() {
    broker[0] = '\0';
    ticker[0] = '\0';
    quantity = 0;
    price = 0;
    date[0] = '\0';
}

// constructor that stores one trade
Trade::Trade(const char b[], const char t[], int q, double p, const char d[]) {
    copyText(broker, b, TEXT_LEN);
    copyText(ticker, t, TEXT_LEN);
    quantity = q;
    price = p;
    copyText(date, d, DATE_LEN);
}

// returns true if the trade is a buy
bool Trade::isBuy() {
    return quantity > 0;
}

// returns total money of the trade
double Trade::amount() {
    int q = quantity;
    if (q < 0) q = -q;
    return q * price;
}

