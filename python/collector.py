import os

from sample_data import SAMPLE_TRADES, SAMPLE_PRICES


def _live_trades():
    """Placeholder for live broker APIs (Groww / Zerodha).
    Returns None when no credentials are configured, which triggers the
    sample-data fallback. Implement per-broker calls here where permitted."""
    if not (os.getenv("GROWW_TOKEN") or os.getenv("ZERODHA_TOKEN")):
        return None
    
    return None


def _live_prices(tickers):
    """Placeholder for a market-price source. None -> use sample prices."""
    return None


def get_trades():
    trades = _live_trades()
    if trades is None:
        print("[collector] live data unavailable, using sample trades")
        trades = SAMPLE_TRADES
    return trades


def get_prices(tickers):
    prices = _live_prices(tickers)
    if prices is None:
        print("[collector] live prices unavailable, using sample prices")
        prices = SAMPLE_PRICES
    return {t: prices[t] for t in tickers if t in prices}
