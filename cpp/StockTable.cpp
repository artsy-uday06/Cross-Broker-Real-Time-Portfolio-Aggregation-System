#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "StockTable.h"
using namespace std;

// constructor: makes an empty stock table
StockTable::StockTable() {
    count = 0;
    lastSteps = 0;
}

// destructor: deletes all stocks
StockTable::~StockTable() {
    for (int i = 0; i < count; i++)
        delete list[i];
}

// BINARY SEARCH: finds a ticker in the sorted stock array
int StockTable::binarySearch(const char ticker[]) {
    int low = 0;
    int high = count - 1;
    lastSteps = 0;
    while (low <= high) {
        lastSteps++;
        int mid = (low + high) / 2;
        int result = strcmp(list[mid]->ticker, ticker);
        if (result == 0)
            return mid;
        else if (result < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// finds a stock, or inserts it at the sorted position if it is new
Stock* StockTable::getOrCreate(const char ticker[]) {
    int pos = binarySearch(ticker);
    if (pos != -1) return list[pos];
    if (count == MAX_STOCKS) return NULL;

    int i = count - 1;
    while (i >= 0 && strcmp(list[i]->ticker, ticker) > 0) {
        list[i + 1] = list[i];
        i--;
    }
    list[i + 1] = new Stock(ticker);
    count++;
    return list[i + 1];
}

// sets the latest price of a stock
void StockTable::setPrice(const char ticker[], double price) {
    Stock* s = getOrCreate(ticker);
    if (s != NULL) s->price = price;
}

// returns the price of a stock (0 if not found)
double StockTable::getPrice(const char ticker[]) {
    int pos = binarySearch(ticker);
    if (pos == -1) return 0;
    return list[pos]->price;
}

// prints the prices of all stocks
void StockTable::showPrices() {
    cout << "\n=== STOCK PRICES (sorted by ticker) ===\n";
    for (int i = 0; i < count; i++) {
        printf("%-12s%10.2f\n", list[i]->ticker, list[i]->price);
    }
}
