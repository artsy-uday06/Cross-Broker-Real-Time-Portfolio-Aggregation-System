#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Portfolio.h"
#include "RecordParser.h"
#include "TcpReceiver.h"
#include "TextUtil.h"
using namespace std;

const int LINE_SIZE = 256;

// handles one line received from Python
void handleRecord(const char line[], Portfolio& pf) {
    char type[20];
    char ticker[TEXT_LEN];
    getRecordType(line, type);

    if (strcmp(type, "TRADE") == 0) {
        Trade t;
        if (parseTrade(line, t)) pf.addTrade(t);
        else cout << "bad record: " << line << "\n";
    }
    else if (strcmp(type, "PRICE") == 0) {
        double price;
        if (parsePrice(line, ticker, price)) pf.updatePrice(ticker, price);
        else cout << "bad record: " << line << "\n";
    }
    else if (strcmp(type, "INFO") == 0) {
        char name[NAME_LEN];
        char sector[TEXT_LEN];
        if (parseInfo(line, ticker, name, sector)) pf.setInfo(ticker, name, sector);
        else cout << "bad record: " << line << "\n";
    }
    else if (strcmp(type, "CANDLE") == 0) {
        Candle c;
        if (parseCandle(line, ticker, c)) pf.addCandle(ticker, c);
        else cout << "bad record: " << line << "\n";
    }
}

// terminal menu to show all features
void showMenu(Portfolio& pf) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n----- MENU -----\n"
             << "1. Holdings\n"
             << "2. Broker summary\n"
             << "3. Trade history (linked list)\n"
             << "4. Stock prices (sorted list)\n"
             << "5. Search stock (binary search)\n"
             << "6. Stock details + 1 year prediction\n"
             << "0. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        if (!cin) break;

        if (choice == 1) pf.showHoldings();
        else if (choice == 2) pf.showBrokerSummary();
        else if (choice == 3) pf.showTradeHistory();
        else if (choice == 4) pf.showStockPrices();
        else if (choice == 5 || choice == 6) {
            char ticker[TEXT_LEN];
            cout << "Enter ticker: ";
            cin.width(TEXT_LEN);
            cin >> ticker;
            toUpperText(ticker);
            if (choice == 5) pf.searchStock(ticker);
            else pf.showStockDetails(ticker);
        }
    }
}

// program starts here
int main(int argc, char* argv[]) {
    Portfolio pf;
    char line[LINE_SIZE];
    bool jsonMode = false;
    char fileName[100] = "";
    char searchTicker[TEXT_LEN] = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--json") == 0) jsonMode = true;
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) copyText(fileName, argv[i + 1], 100);
        if (strcmp(argv[i], "--search") == 0 && i + 1 < argc) {
            copyText(searchTicker, argv[i + 1], TEXT_LEN);
            toUpperText(searchTicker);
        }
    }

    if (fileName[0] != '\0') {
        FILE* f = fopen(fileName, "r");
        if (f == NULL) { cout << "cannot open file\n"; return 1; }
        while (fgets(line, LINE_SIZE, f) != NULL) {
            removeNewline(line);
            if (strcmp(line, "END") == 0) break;
            handleRecord(line, pf);
        }
        fclose(f);
    } else {
        TcpReceiver rx;
        if (!rx.start(5050)) return 1;
        if (!rx.acceptClient()) return 1;
        while (rx.readLine(line, LINE_SIZE)) {
            if (strcmp(line, "END") == 0) break;
            handleRecord(line, pf);
        }
    }

    if (jsonMode) {
        pf.printJson(searchTicker);
    } else {
        pf.showHoldings();
        pf.showBrokerSummary();
        showMenu(pf);
    }
    return 0;
}
