"""formatter.py  -  Owner: Parth
Turns collected data into the delimiter-separated text records
described in docs/RECORD_FORMAT.md."""

DELIM = "|"


def _clean(text):
    """Remove the delimiter and newlines so a field can never break a record."""
    return str(text).replace(DELIM, " ").replace("\n", " ").replace("\r", " ").strip()


def format_trade(trade):
    """trade: dict with broker, ticker, quantity (signed), price, date."""
    return DELIM.join([
        "TRADE",
        _clean(trade["broker"]),
        _clean(trade["ticker"]).upper(),
        str(int(trade["quantity"])),
        f"{float(trade['price']):.2f}",
        _clean(trade["date"]),
    ])


def format_price(ticker, price):
    return DELIM.join(["PRICE", _clean(ticker).upper(), f"{float(price):.2f}"])


def format_end():
    return "END"
