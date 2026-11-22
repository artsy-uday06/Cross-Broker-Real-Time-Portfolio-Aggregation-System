#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TradeList.h"
using namespace std;

// constructor: makes an empty linked list
TradeList::TradeList() {
    head = NULL;
    count = 0;
}

// destructor: deletes all nodes of the linked list
TradeList::~TradeList() {
    Node* cur = head;
    while (cur != NULL) {
        Node* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
}

// LINKED LIST: inserts a trade so the list stays sorted by date
void TradeList::insertInOrder(Trade t) {
    Node* newNode = new Node;
    newNode->data = t;
    newNode->next = NULL;

    if (head == NULL || strcmp(t.date, head->data.date) < 0) {
        newNode->next = head;
        head = newNode;
    }
    else {
        Node* cur = head;
        while (cur->next != NULL && strcmp(cur->next->data.date, t.date) <= 0) {
            cur = cur->next;
        }
        newNode->next = cur->next;
        cur->next = newNode;
    }
    count++;
}

// LINKED LIST: goes through every node and prints the trades
void TradeList::display() {
    cout << "\n=== TRADE HISTORY (" << count << " trades) ===\n";
    Node* cur = head;
    while (cur != NULL) {
        int qty = cur->data.quantity;
        if (qty < 0) qty = -qty;
        printf("%-12s%-10s%-11s%-6s%6d%12.2f\n",
               cur->data.date, cur->data.broker, cur->data.ticker,
               cur->data.isBuy() ? "BUY" : "SELL", qty, cur->data.price);
        cur = cur->next;
    }
}
