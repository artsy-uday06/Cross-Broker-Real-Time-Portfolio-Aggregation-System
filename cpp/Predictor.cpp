#include <iostream>
#include "Predictor.h"
using namespace std;

// square root using Newton's method (no math.h)
double mySqrt(double x) {
    if (x <= 0) return 0;
    double g = x;
    for (int i = 0; i < 40; i++)
        g = (g + x / g) / 2;
    return g;
}

// finds average daily change and daily ups and downs from the candles
bool calcTrend(Stock* s, double& change, double& sigma) {
    if (s->candleCount < 10) return false;

    int n = s->candleCount - 1;

    change = (s->candles[n].close - s->candles[0].close) / n;

    double dailyPercent[MAX_CANDLES];
    double sum = 0;
    for (int i = 1; i < s->candleCount; i++) {
        dailyPercent[i - 1] = (s->candles[i].close - s->candles[i - 1].close) / s->candles[i - 1].close;
        sum = sum + dailyPercent[i - 1];
    }
    double avg = sum / n;

    double sq = 0;
    for (int i = 0; i < n; i++)
        sq = sq + (dailyPercent[i] - avg) * (dailyPercent[i] - avg);
    sigma = mySqrt(sq / (n - 1));
    return true;
}

// predicts the price after the given number of days
double projectPrice(double priceNow, double change, double sigma, int days, int mode) {
    double expected = priceNow + change * days;
    if (expected < 0) expected = 0;
    if (mode == 0) return expected;

    double spread = sigma * mySqrt(days);
    if (spread > 0.9) spread = 0.9;
    return expected * (1 + mode * spread);
}
