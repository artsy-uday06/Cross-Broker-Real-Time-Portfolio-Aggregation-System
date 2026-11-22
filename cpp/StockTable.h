#ifndef STOCKTABLE_H
#define STOCKTABLE_H
#include "Stock.h"

const int MAX_STOCKS = 50;

class StockTable {
public:
    Stock* list[MAX_STOCKS];
    int count;
    int lastSteps;

    StockTable();
    ~StockTable();
    int    binarySearch(const char ticker[]);
    Stock* getOrCreate(const char ticker[]);
    void   setPrice(const char ticker[], double price);
    double getPrice(const char ticker[]);
    void   showPrices();
};
#endif
