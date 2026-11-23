import random
from datetime import date, timedelta

START = date(2025, 10, 1)
END = date(2026, 3, 31)


# returns all Monday to Friday dates between two dates
def weekdays(start, end):
    days = []
    d = start
    while d <= end:
        if d.weekday() < 5:
            days.append(d)
        d = d + timedelta(days=1)
    return days


# makes sample candles for one stock
def make_candles(ticker, trades, current_price):
    days = weekdays(START, END)
    n = len(days)

    anchors = {}
    my_trades = [t for t in trades if t["ticker"] == ticker]
    anchors[0] = my_trades[0]["price"] * 0.92
    for t in my_trades:
        for i in range(n):
            if str(days[i]) == t["date"]:
                anchors[i] = t["price"]
    anchors[n - 1] = current_price
    points = sorted(anchors.keys())

    base = [0] * n
    for k in range(len(points) - 1):
        a = points[k]
        b = points[k + 1]
        for i in range(a, b + 1):
            base[i] = anchors[a] + (anchors[b] - anchors[a]) * (i - a) / (b - a)

    rng = random.Random(ticker)
    walk = [0.0] * n
    for i in range(1, n):
        walk[i] = walk[i - 1] + rng.gauss(0, 0.011)
    noise = [0.0] * n
    for k in range(len(points) - 1):
        a = points[k]
        b = points[k + 1]
        for i in range(a, b + 1):
            line = walk[a] + (walk[b] - walk[a]) * (i - a) / (b - a)
            noise[i] = walk[i] - line

    candles = []
    prev_close = None
    for i in range(n):
        close = base[i] * (1 + noise[i])
        if prev_close is None:
            open_ = close
        else:
            open_ = prev_close * (1 + rng.uniform(-0.004, 0.004))
        high = max(open_, close) * (1 + rng.uniform(0.001, 0.010))
        low = min(open_, close) * (1 - rng.uniform(0.001, 0.010))
        candles.append({"date": str(days[i]), "open": round(open_, 2), "high": round(high, 2),
                        "low": round(low, 2), "close": round(close, 2)})
        prev_close = close
    return candles
