# Cross-Broker Real-Time Portfolio Aggregation System  (Phase II)

## Who owns which file 

| Member | Report task | Files |
| Uday Raj Singh (Lead) | C++ portfolio class, record format | `cpp/Portfolio.h/.cpp`, `cpp/TcpReceiver.h/.cpp`, `cpp/main.cpp` |
| Soumy Jalal | Python data collection (sample-data fallback) | `python/sample_data.py`, `python/collector.py`, `python/main.py` |
| Saksham Jain  | C++ classes for trades and holdings | `cpp/Trade.h/.cpp`, `cpp/Holding.h/.cpp`, `cpp/RecordParser.h/.cpp` |
| Parth| Record format, Python formatting | `docs/RECORD_FORMAT.md`, `python/formatter.py`, `python/sender.py` |

## Run

Terminal 1 (C++ listens first):
    cd cpp && make && ./portfolio
Terminal 2:
    cd python && python main.py

Test without sockets:
    cd python && python main.py --dump records.txt
    cd cpp && ./portfolio --file ../python/records.txt

On Windows (MinGW) add `-lws2_32` to the g++ command.

## Phase III placeholders
Portfolio currently keeps trades in a dynamic array. The Singly Linked List
and BST  will replace it, so only Portfolio.cpp changes.
