"""sample_data.py  -  Owner: Soumy Jalal
Representative sample data in the same shape live broker data would have.
Used when live broker access is unavailable."""

SAMPLE_TRADES = [
    {"broker": "Groww",   "ticker": "TCS",       "quantity": 10,  "price": 3850.50, "date": "2026-01-12"},
    {"broker": "Groww",   "ticker": "INFY",      "quantity": 25,  "price": 1540.00, "date": "2026-01-20"},
    {"broker": "Groww",   "ticker": "ITC",       "quantity": 100, "price": 425.30,  "date": "2026-02-05"},
    {"broker": "Zerodha", "ticker": "TCS",       "quantity": 5,   "price": 3900.00, "date": "2026-02-11"},
    {"broker": "Zerodha", "ticker": "RELIANCE",  "quantity": 15,  "price": 2890.75, "date": "2026-02-18"},
    {"broker": "Zerodha", "ticker": "HDFCBANK",  "quantity": 20,  "price": 1650.00, "date": "2026-02-25"},
    {"broker": "Zerodha", "ticker": "TCS",       "quantity": -4,  "price": 4010.00, "date": "2026-03-02"},
    {"broker": "Groww",   "ticker": "INFY",      "quantity": -10, "price": 1610.40, "date": "2026-03-09"},
]

SAMPLE_PRICES = {
    "TCS": 4120.75,
    "INFY": 1585.20,
    "ITC": 438.90,
    "RELIANCE": 2955.10,
    "HDFCBANK": 1702.35,
}

