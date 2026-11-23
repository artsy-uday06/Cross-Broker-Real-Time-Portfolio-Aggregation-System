from sample_data import SAMPLE_TRADES, SAMPLE_PRICES, SAMPLE_INFO
from sample_history import make_candles


# returns the list of trades
def get_trades():
    print("[collector] using sample trades")
    return SAMPLE_TRADES


# returns the latest price of each ticker
def get_prices(tickers):
    print("[collector] using sample prices")
    prices = {}
    for t in tickers:
        if t in SAMPLE_PRICES:
            prices[t] = SAMPLE_PRICES[t]
    return prices


# returns (company name, sector) of a stock
def get_info(ticker):
    if ticker in SAMPLE_INFO:
        return SAMPLE_INFO[ticker]
    return (ticker, "-")


# returns the candle history of a stock
def get_history(ticker, trades, current_price):
    return make_candles(ticker, trades, current_price)
