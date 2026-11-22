#ifndef PORTFOLIO_H
#define PORTFOLIO_H
#include "Trade.h"
#include "Holding.h"
#include "TradeList.h"
#include "StockTable.h"

class Portfolio {
public:
    TradeList trades;
    StockTable prices;
    Holding* holdings[100];
    int holdingCount;

    Portfolio();
    ~Portfolio();

    Holding* findHolding(const char broker[], const char ticker[]);
    void addTrade(Trade t);
    void updatePrice(const char ticker[], double price);
    void setInfo(const char ticker[], const char name[], const char sector[]);
    void addCandle(const char ticker[], Candle c);

    double totalInvested();
    double totalValue();
    double brokerInvested(const char broker[]);
    double brokerValue(const char broker[]);
    int    getBrokerNames(char names[][TEXT_LEN]);

    void showHoldings();
    void showBrokerSummary();
    void showTradeHistory();
    void showStockPrices();
    void searchStock(const char ticker[]);
    void showStockDetails(const char ticker[]);
    void printJson(const char searchTicker[]);
};
#endif
