"""main.py  -  Owner: Soumy Jalal & parth
Python entry point: collect -> format -> send.

  python main.py              send to the C++ process over TCP (127.0.0.1:5050)
  python main.py --dump FILE  write the records to FILE instead (for testing the C++ parser)
"""

import sys

from collector import get_trades, get_prices
from formatter import format_trade, format_price, format_end
from sender import send_records


def build_records():
    trades = get_trades()
    tickers = sorted({t["ticker"] for t in trades})
    prices = get_prices(tickers)

    records = [format_trade(t) for t in trades]
    records += [format_price(tk, p) for tk, p in prices.items()]
    records.append(format_end())
    return records

if __name__ == "__main__":
    recs = build_records()
    if len(sys.argv) == 3 and sys.argv[1] == "--dump":
        with open(sys.argv[2], "w", encoding="utf-8") as f:
            f.write("\n".join(recs) + "\n")
        print(f"[main] wrote {len(recs)} records to {sys.argv[2]}")
    else:
        send_records(recs)

