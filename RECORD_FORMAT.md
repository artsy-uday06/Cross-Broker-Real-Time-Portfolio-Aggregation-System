# Record Format 

One record per line, fields separated by `|`, UTF-8, `\n` terminated.

| Record | Format | Notes |
|--------|--------|-------|
| Trade  | `TRADE\|broker\|ticker\|quantity\|price\|date` | quantity > 0 = BUY, quantity < 0 = SELL. date = YYYY-MM-DD |
| Price  | `PRICE\|ticker\|price` | latest market price |
| End    | `END` | tells C++ the stream is finished |
| News   | `NEWS\|ticker\|headline` | RESERVED - Phase III |

Examples
```
TRADE|Groww|TCS|10|3850.50|2026-01-12
TRADE|Zerodha|TCS|-4|4010.00|2026-03-02
PRICE|TCS|4120.75
END
```
The `|` character is removed from any text field by the Python formatter.
