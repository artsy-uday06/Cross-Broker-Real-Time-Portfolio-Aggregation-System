#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "RecordParser.h"
using namespace std;

// splits a line at | into parts
int splitLine(const char line[], char parts[][FIELD_LEN], int maxParts) {
    int count = 0;
    int pos = 0;
    for (int i = 0; ; i++) {
        char ch = line[i];
        if (ch == '|' || ch == '\0') {
            if (count < maxParts) {
                parts[count][pos] = '\0';
                count++;
            }
            pos = 0;
            if (ch == '\0') break;
        }
        else if (count < maxParts && pos < FIELD_LEN - 1) {
            parts[count][pos] = ch;
            pos++;
        }
    }
    return count;
}

// gets the first word of the line (TRADE, PRICE, INFO, CANDLE)
void getRecordType(const char line[], char type[]) {
    char p[10][FIELD_LEN];
    splitLine(line, p, 10);
    copyText(type, p[0], 20);
}

// reads a TRADE line
bool parseTrade(const char line[], Trade& result) {
    char p[10][FIELD_LEN];
    int n = splitLine(line, p, 10);
    if (n != 6 || strcmp(p[0], "TRADE") != 0) return false;
    int qty = atoi(p[3]);
    double price = atof(p[4]);
    if (qty == 0 || price <= 0) return false;
    result = Trade(p[1], p[2], qty, price, p[5]);
    return true;
}

// reads a PRICE line
bool parsePrice(const char line[], char ticker[], double& price) {
    char p[10][FIELD_LEN];
    int n = splitLine(line, p, 10);
    if (n != 3 || strcmp(p[0], "PRICE") != 0) return false;
    copyText(ticker, p[1], TEXT_LEN);
    price = atof(p[2]);
    return price > 0;
}

// copies text without quotes and backslash
void copyClean(char dest[], const char src[], int size) {
    int j = 0;
    for (int i = 0; src[i] != '\0' && j < size - 1; i++) {
        if (src[i] != '"' && src[i] != '\\') {
            dest[j] = src[i];
            j++;
        }
    }
    dest[j] = '\0';
}

// reads an INFO line
bool parseInfo(const char line[], char ticker[], char name[], char sector[]) {
    char p[10][FIELD_LEN];
    int n = splitLine(line, p, 10);
    if (n != 4 || strcmp(p[0], "INFO") != 0) return false;
    copyText(ticker, p[1], TEXT_LEN);
    copyClean(name, p[2], NAME_LEN);
    copyClean(sector, p[3], TEXT_LEN);
    return true;
}

// reads a CANDLE line
bool parseCandle(const char line[], char ticker[], Candle& c) {
    char p[10][FIELD_LEN];
    int n = splitLine(line, p, 10);
    if (n != 7 || strcmp(p[0], "CANDLE") != 0) return false;
    copyText(ticker, p[1], TEXT_LEN);
    copyText(c.date, p[2], DATE_LEN);
    c.open = atof(p[3]);
    c.high = atof(p[4]);
    c.low = atof(p[5]);
    c.close = atof(p[6]);
    return c.open > 0 && c.high > 0 && c.low > 0 && c.close > 0;
}
