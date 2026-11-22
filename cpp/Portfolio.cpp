#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Portfolio.h"
#include "Predictor.h"
using namespace std;

// constructor of Portfolio
Portfolio::Portfolio() {
    holdingCount = 0;
}

// destructor: deletes all holdings
Portfolio::~Portfolio() {
    for (int i = 0; i < holdingCount; i++)
        delete holdings[i];
}

// linear search: finds the holding of a stock at a broker
Holding* Portfolio::findHolding(const char broker[], const char ticker[]) {
    for (int i = 0; i < holdingCount; i++) {
        if (strcmp(holdings[i]->broker, broker) == 0 && strcmp(holdings[i]->ticker, ticker) == 0)
            return holdings[i];
    }
    return NULL;
}

// adds a trade to the linked list and updates the holding
void Portfolio::addTrade(Trade t) {
    trades.insertInOrder(t);

    Holding* h = findHolding(t.broker, t.ticker);
    if (h == NULL) {
        if (holdingCount == 100) return;
        h = new Holding(t.broker, t.ticker);
        holdings[holdingCount] = h;
        holdingCount++;
    }
    h->applyTrade(t);
}

// updates the price of a stock and of its holdings
void Portfolio::updatePrice(const char ticker[], double price) {
    prices.setPrice(ticker, price);
    for (int i = 0; i < holdingCount; i++) {
        if (strcmp(holdings[i]->ticker, ticker) == 0)
            holdings[i]->currentPrice = price;
    }
}

// stores company name and sector of a stock
void Portfolio::setInfo(const char ticker[], const char name[], const char sector[]) {
    Stock* s = prices.getOrCreate(ticker);
    if (s != NULL) {
        copyText(s->name, name, NAME_LEN);
        copyText(s->sector, sector, TEXT_LEN);
    }
}

// adds a candle to a stock
void Portfolio::addCandle(const char ticker[], Candle c) {
    Stock* s = prices.getOrCreate(ticker);
    if (s != NULL) s->addCandle(c);
}

// total money invested in all holdings
double Portfolio::totalInvested() {
    double sum = 0;
    for (int i = 0; i < holdingCount; i++) sum += holdings[i]->invested;
    return sum;
}

// total current value of all holdings
double Portfolio::totalValue() {
    double sum = 0;
    for (int i = 0; i < holdingCount; i++) sum += holdings[i]->currentValue();
    return sum;
}

// money invested at one broker
double Portfolio::brokerInvested(const char broker[]) {
    double sum = 0;
    for (int i = 0; i < holdingCount; i++)
        if (strcmp(holdings[i]->broker, broker) == 0) sum += holdings[i]->invested;
    return sum;
}

// current value at one broker
double Portfolio::brokerValue(const char broker[]) {
    double sum = 0;
    for (int i = 0; i < holdingCount; i++)
        if (strcmp(holdings[i]->broker, broker) == 0) sum += holdings[i]->currentValue();
    return sum;
}

// collects the different broker names
int Portfolio::getBrokerNames(char names[][TEXT_LEN]) {
    int n = 0;
    for (int i = 0; i < holdingCount; i++) {
        bool seen = false;
        for (int j = 0; j < n; j++)
            if (strcmp(names[j], holdings[i]->broker) == 0) seen = true;
        if (!seen && n < 10) {
            copyText(names[n], holdings[i]->broker, TEXT_LEN);
            n++;
        }
    }
    return n;
}

// prints the holdings table
void Portfolio::showHoldings() {
    cout << "\n=== HOLDINGS ===\n";
    printf("%-10s%-11s%6s%14s%14s%14s\n", "Broker", "Stock", "Qty", "Invested", "Current", "P/L");
    for (int i = 0; i < holdingCount; i++) {
        Holding* h = holdings[i];
        if (h->quantity == 0) continue;
        printf("%-10s%-11s%6d%14.2f%14.2f%14.2f\n", h->broker, h->ticker, h->quantity,
               h->invested, h->currentValue(), h->profitLoss());
    }
}

// prints broker wise profit or loss
void Portfolio::showBrokerSummary() {
    char names[10][TEXT_LEN];
    int n = getBrokerNames(names);

    cout << "\n=== BROKER SUMMARY ===\n";
    for (int i = 0; i < n; i++) {
        double inv = brokerInvested(names[i]);
        double val = brokerValue(names[i]);
        printf("%-10s invested %12.2f  value %12.2f  P/L %12.2f\n", names[i], inv, val, val - inv);
    }
    printf("%-10s invested %12.2f  value %12.2f  P/L %12.2f\n", "TOTAL",
           totalInvested(), totalValue(), totalValue() - totalInvested());
}

// prints all trades (linked list)
void Portfolio::showTradeHistory() {
    trades.display();
}

// prints the stock prices (sorted list)
void Portfolio::showStockPrices() {
    prices.showPrices();
}

// BINARY SEARCH: searches a stock by ticker and prints it
void Portfolio::searchStock(const char ticker[]) {
    int pos = prices.binarySearch(ticker);
    if (pos == -1) {
        cout << ticker << " not found (binary search took " << prices.lastSteps << " steps).\n";
        return;
    }
    Stock* s = prices.list[pos];
    printf("Found at index %d in %d steps : %s  price = %.2f\n", pos, prices.lastSteps, s->ticker, s->price);
    for (int i = 0; i < holdingCount; i++) {
        if (strcmp(holdings[i]->ticker, ticker) == 0 && holdings[i]->quantity > 0)
            printf("   %s holds %d shares, P/L = %.2f\n", holdings[i]->broker,
                   holdings[i]->quantity, holdings[i]->profitLoss());
    }
}

// prints stock details and the 1 year prediction
void Portfolio::showStockDetails(const char ticker[]) {
    int pos = prices.binarySearch(ticker);
    if (pos == -1) {
        cout << ticker << " not found.\n";
        return;
    }
    Stock* s = prices.list[pos];
    printf("\n%s (%s)  sector: %s\n", s->name, s->ticker, s->sector);
    printf("Current price: %.2f\n", s->price);

    cout << "\nLast 5 candles (date, open, high, low, close):\n";
    int start = s->candleCount - 5;
    if (start < 0) start = 0;
    for (int i = start; i < s->candleCount; i++) {
        printf("  %s  %9.2f%9.2f%9.2f%9.2f\n", s->candles[i].date, s->candles[i].open,
               s->candles[i].high, s->candles[i].low, s->candles[i].close);
    }

    double change, sigma;
    if (!calcTrend(s, change, sigma)) {
        cout << "\nNot enough history for a prediction.\n";
        return;
    }
    cout << "\n--- 1 YEAR PREDICTION (if the same trend continues) ---\n";
    printf("Average change per day : %.2f rupees (%.2f %%)\n", change, change / s->price * 100);
    printf("Daily ups and downs    : %.2f %%\n", sigma * 100);
    printf("Expected price         : %.2f\n", projectPrice(s->price, change, sigma, YEAR_DAYS, 0));
    printf("Best case              : %.2f\n", projectPrice(s->price, change, sigma, YEAR_DAYS, 1));
    printf("Worst case             : %.2f\n", projectPrice(s->price, change, sigma, YEAR_DAYS, -1));
    cout << "(simple maths on sample data - not financial advice)\n";
}

// prints everything as JSON for the website
void Portfolio::printJson(const char searchTicker[]) {
    printf("{\"totals\":{\"invested\":%.2f,\"value\":%.2f,\"profitLoss\":%.2f},",
           totalInvested(), totalValue(), totalValue() - totalInvested());

    printf("\"holdings\":[");
    bool first = true;
    for (int i = 0; i < holdingCount; i++) {
        Holding* h = holdings[i];
        if (h->quantity == 0) continue;
        if (!first) printf(",");
        first = false;
        printf("{\"broker\":\"%s\",\"ticker\":\"%s\",\"quantity\":%d,\"invested\":%.2f,"
               "\"currentValue\":%.2f,\"profitLoss\":%.2f}",
               h->broker, h->ticker, h->quantity, h->invested, h->currentValue(), h->profitLoss());
    }

    printf("],\"brokers\":[");
    char names[10][TEXT_LEN];
    int n = getBrokerNames(names);
    for (int i = 0; i < n; i++) {
        if (i > 0) printf(",");
        double inv = brokerInvested(names[i]);
        double val = brokerValue(names[i]);
        printf("{\"broker\":\"%s\",\"invested\":%.2f,\"value\":%.2f,\"profitLoss\":%.2f}",
               names[i], inv, val, val - inv);
    }

    printf("],\"trades\":[");
    Node* cur = trades.head;
    while (cur != NULL) {
        Trade t = cur->data;
        printf("{\"date\":\"%s\",\"broker\":\"%s\",\"ticker\":\"%s\",\"quantity\":%d,"
               "\"side\":\"%s\",\"price\":%.2f,\"amount\":%.2f}",
               t.date, t.broker, t.ticker, t.quantity, t.isBuy() ? "BUY" : "SELL", t.price, t.amount());
        cur = cur->next;
        if (cur != NULL) printf(",");
    }

    printf("],\"stocks\":[");
    for (int i = 0; i < prices.count; i++) {
        Stock* s = prices.list[i];
        if (i > 0) printf(",");
        printf("{\"ticker\":\"%s\",\"name\":\"%s\",\"sector\":\"%s\",\"price\":%.2f,\"candles\":[",
               s->ticker, s->name, s->sector, s->price);

        for (int j = 0; j < s->candleCount; j++) {
            if (j > 0) printf(",");
            printf("{\"d\":\"%s\",\"o\":%.2f,\"h\":%.2f,\"l\":%.2f,\"c\":%.2f}",
                   s->candles[j].date, s->candles[j].open, s->candles[j].high,
                   s->candles[j].low, s->candles[j].close);
        }

        printf("],\"prediction\":");
        double change, sigma;
        if (calcTrend(s, change, sigma)) {
            printf("{\"days\":%d,\"dailyTrend\":%.4f,\"dailyRisk\":%.4f,\"expected\":%.2f,\"high\":%.2f,\"low\":%.2f,\"path\":[",
                   YEAR_DAYS, change / s->price * 100, sigma * 100,
                   projectPrice(s->price, change, sigma, YEAR_DAYS, 0),
                   projectPrice(s->price, change, sigma, YEAR_DAYS, 1),
                   projectPrice(s->price, change, sigma, YEAR_DAYS, -1));
            for (int d = 21; d <= YEAR_DAYS; d += 21) {
                if (d > 21) printf(",");
                printf("{\"day\":%d,\"expected\":%.2f,\"high\":%.2f,\"low\":%.2f}", d,
                       projectPrice(s->price, change, sigma, d, 0),
                       projectPrice(s->price, change, sigma, d, 1),
                       projectPrice(s->price, change, sigma, d, -1));
            }
            printf("]}");
        } else {
            printf("null");
        }
        printf("}");
    }

    printf("],\"search\":");
    if (searchTicker[0] == '\0') {
        printf("null");
    } else {
        int pos = prices.binarySearch(searchTicker);
        printf("{\"query\":\"%s\",\"found\":%s,\"position\":%d,\"total\":%d,\"steps\":%d}",
               searchTicker, pos != -1 ? "true" : "false", pos + 1, prices.count, prices.lastSteps);
    }
    printf("}\n");
}
