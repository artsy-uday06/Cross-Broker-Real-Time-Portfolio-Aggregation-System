#ifndef PREDICTOR_H
#define PREDICTOR_H
#include "Stock.h"

const int YEAR_DAYS = 252;

double mySqrt(double x);

bool calcTrend(Stock* s, double& change, double& sigma);

double projectPrice(double priceNow, double change, double sigma, int days, int mode);
#endif
