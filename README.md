# Cross-Broker Portfolio Aggregator — Local Website Integration

This package keeps the original Python and C++ project and adds a browser frontend connected to the **real Python -> TCP -> C++ pipeline**. It is currently configured for the supplied sample data; it does not connect to live Groww/Zerodha accounts or fetch live prices/news.

## Requirements

- Python 3.9+ (standard library only; no pip packages required)
- `g++` available in PATH (MinGW-w64 on Windows is fine)
- A modern browser such as Chrome or Edge

## Run on Windows (no terminal window)

1. Install Python 3 and a C++ compiler (`g++`, such as MinGW-w64) once if they are not installed. The existing C++ source is compiled locally when the launcher starts or after source files change.
2. Double-click **`Open Portfolio Website.vbs`**. If Windows warns about a downloaded script, right-click it, choose **Properties**, check **Unblock** if that option is shown, and click **Apply**. Only do this for the project file you have inspected and trust.
3. The backend starts invisibly and your default browser opens `http://127.0.0.1:8000` automatically.
4. Keep the project folder in place while using the website. The backend runs in the background. Startup errors are written to `website.log`.

Important: opening a plain HTML file cannot execute Python or C++ by itself. This one-click launcher hides the local backend process so you do not need to open Command Prompt manually. Python and `g++` are still required; this is not yet a fully self-contained Windows `.exe` installer.

## Run on macOS/Linux

From this folder run:

```bash
python3 web_server.py
```

Then open `http://127.0.0.1:8000` in your browser.

## How the connected architecture works

1. `python/sample_data.py` supplies the current example trades and prices.
2. `python/collector.py` selects the sample data fallback.
3. `python/formatter.py` creates the agreed `TRADE|...`, `PRICE|...`, and `END` records.
4. `python/sender.py` sends those records over TCP to `127.0.0.1:5050`.
5. `cpp/TcpReceiver.cpp` receives the records and `cpp/RecordParser.cpp` parses them.
6. The original `Trade`, `Holding`, and `Portfolio` C++ classes calculate holdings and profit/loss.
7. `Portfolio::printJson()` exposes those same C++ results as JSON for the browser.
8. `web/index.html` displays the results in Dashboard, Holdings, Trade History and Broker Comparison.

The local web server starts the C++ TCP receiver and runs the existing Python `main.py` for each data refresh. The TCP sender/receiver path is used; the browser does not replace the C++ calculations.

## Making code changes

- **Change sample trades/prices:** edit `python/sample_data.py`, save, then click **Refresh data** in the website.
- **Change Python collection/formatting/sending:** edit the existing Python files, save, then refresh. The server invokes `python/main.py` from disk each refresh.
- **Change C++ calculations or output:** edit the `.cpp`/`.h` files and save, then refresh. The server detects newer C++ source files and recompiles before processing.
- **Change the frontend appearance/layout:** edit `web/index.html`, save, then refresh the browser.

The website intentionally has **no add-stock, add-trade, or manual-price editing controls**. The supplied sample dataset is the source of truth for this phase. Browser refresh uses `Cache-Control: no-store` to help show saved code changes.

## Website pages

- Dashboard: total value, invested amount, profit/loss, open positions, broker allocation, top holdings
- Holdings: open positions with ticker/broker search
- Trade history: sample transactions with search
- Broker comparison: broker-level invested value, market value, profit/loss and return
- Market news: interface placeholder only. The original ZIP does not currently implement news collection, so cards are explicitly illustrative and not live news.

## Original terminal workflow remains available

You can still use the original terminal programs directly:

```bash
# Terminal 1: C++ TCP receiver
cd cpp
# Windows: build manually with -lws2_32 if using MinGW
make
./portfolio

# Terminal 2
cd python
python main.py
```

On Windows, use `g++` directly or a compatible `make` environment if `make` is not installed. The website launcher builds the C++ executable automatically and includes `-lws2_32` on Windows.

## Important limitations

- Sample data only; no broker authentication or live market price API yet.
- News is not implemented in the original source and is a placeholder page.
- This is a local development website bound to `127.0.0.1`, not a public production server.


## Professional multi-page interface
The website opens on a dedicated Home page. Use the left navigation or Home shortcuts to open Dashboard, Positions & Holdings, Trade History, Broker Comparison and Market News as separate views. Navigation updates the URL hash, so browser Back/Forward works. The page continues to fetch portfolio results from the local Python bridge and original C++ processing pipeline.
