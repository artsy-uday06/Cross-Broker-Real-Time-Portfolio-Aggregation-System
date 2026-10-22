
#ifndef TRADE_H
#define TRADE_H
#include <string>

class Trade {
private:
    std::string broker_;
    std::string ticker_;
    int         quantity_;   
    double      price_;
    std::string date_;

public:
    Trade(const std::string& broker, const std::string& ticker,
          int quantity, double price, const std::string& date);

    const std::string& broker() const { return broker_; }
    const std::string& ticker() const { return ticker_; }
    int                quantity() const { return quantity_; }
    double             price() const { return price_; }
    const std::string& date() const { return date_; }

    bool   isBuy() const { return quantity_ > 0; }
    double amount() const;   // absolute trade value
    void   print() const;
};
#endif
