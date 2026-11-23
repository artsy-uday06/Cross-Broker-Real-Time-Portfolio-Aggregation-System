import sys

from collector import get_trades, get_prices, get_info, get_history
from formatter import format_trade, format_price, format_info, format_candle, format_end
from sender import send_records


# collects the data and makes all the lines to send
def build_records():
    trades = get_trades()

    tickers = sorted(set(t["ticker"] for t in trades))
    prices = get_prices(tickers)

    records = []
    for t in trades:
        records.append(format_trade(t))

    for ticker in prices:
        name, sector = get_info(ticker)
        records.append(format_info(ticker, name, sector))
        records.append(format_price(ticker, prices[ticker]))
        for c in get_history(ticker, trades, prices[ticker]):
            records.append(format_candle(ticker, c))

    records.append(format_end())
    return records


if __name__ == "__main__":
    records = build_records()
    if len(sys.argv) == 3 and sys.argv[1] == "--dump":
        f = open(sys.argv[2], "w")
        f.write("\n".join(records) + "\n")
        f.close()
        print("[main] wrote", len(records), "records to", sys.argv[2])
    else:
        send_records(records)
