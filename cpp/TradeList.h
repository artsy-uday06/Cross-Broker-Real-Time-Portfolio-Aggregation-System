#ifndef TRADELIST_H
#define TRADELIST_H
#include "Trade.h"

struct Node {
    Trade data;
    Node* next;
};

class TradeList {
public:
    Node* head;
    int count;

    TradeList();
    ~TradeList();
    void insertInOrder(Trade t);
    void display();
};
#endif
