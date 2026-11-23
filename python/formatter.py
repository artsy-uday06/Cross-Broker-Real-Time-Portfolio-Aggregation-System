# removes | from text because | is the separator
def clean(text):
    return str(text).replace("|", " ")


# makes a TRADE line
def format_trade(trade):
    return ("TRADE|" + clean(trade["broker"]) + "|" + trade["ticker"].upper() + "|" +
            str(trade["quantity"]) + "|" + str(trade["price"]) + "|" + trade["date"])


# makes a PRICE line
def format_price(ticker, price):
    return "PRICE|" + ticker.upper() + "|" + str(price)


# makes an INFO line
def format_info(ticker, name, sector):
    return "INFO|" + ticker.upper() + "|" + clean(name) + "|" + clean(sector)


# makes a CANDLE line
def format_candle(ticker, c):
    return ("CANDLE|" + ticker.upper() + "|" + c["date"] + "|" + str(c["open"]) + "|" +
            str(c["high"]) + "|" + str(c["low"]) + "|" + str(c["close"]))


# makes the END line
def format_end():
    return "END"
