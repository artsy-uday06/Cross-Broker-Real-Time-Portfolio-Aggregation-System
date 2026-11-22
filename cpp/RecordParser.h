#ifndef RECORDPARSER_H
#define RECORDPARSER_H
#include "Trade.h"
#include "Stock.h"

const int FIELD_LEN = 80;

int splitLine(const char line[], char parts[][FIELD_LEN], int maxParts);

void getRecordType(const char line[], char type[]);

bool parseTrade(const char line[], Trade& result);
bool parsePrice(const char line[], char ticker[], double& price);
bool parseInfo(const char line[], char ticker[], char name[], char sector[]);
bool parseCandle(const char line[], char ticker[], Candle& c);
#endif
