# Cross-Broker Real-Time Portfolio Aggregation System

A PBL project by to explore how investment information from multiple broker sources can be processed and presented in one portfolio view.

## Problem
Investments held across multiple broker platforms can be inconvenient to review together. This project proposes a unified view of holdings, invested amount, current value, profit/loss, trade history, market information, and relevant stock news.

## Project Objectives
- Collect permitted broker information, initially considering Groww and Zerodha.
- Use Python for data collection and formatting.
- Send data from Python to C++ through a local TCP socket (IPC).
- Use C++ OOP for structured portfolio processing.
- Store chronological trade records in a Singly Linked List.
- Organize ticker information using a Binary Search Tree.
- Calculate holdings, invested amount, current value, and profit/loss.
- Display relevant market information, news, and an approximate one-year projection.

## Team
- Uday Raj Singh-Team Lead
- Soumy Jalal-Team Member
- Saksham Jain-Team Member
- Parth-Team Member

## Planned Technologies
Python, C++, TCP sockets, Object-Oriented Programming, pointers, dynamic memory, string streams, Singly Linked List, and Binary Search Tree.

## Repository Layout
- `python_engine/` planned Python data collection and socket sender
- `cpp_core/` C++ portfolio processing and data structures
- `tests/` basic test files
- `docs/` development notes

## Current Status
The initial C++ portfolio model and sample-data demonstration are being developed. Live broker connectivity, market/news feeds, Python-to-C++ socket communication, and a dashboard are not yet implemented.

## Run the current C++ sample
A C++17 compiler is required.

```bash
g++ cpp_core/main.cpp -o portfolio_demo
./portfolio_demo
```

## Data and Safety Notes
- Use only permitted APIs or authorized data sources.
- Sample data is for demonstration when live access is unavailable.
- Never commit API keys, passwords, or broker credentials.
- The project is for academic portfolio monitoring and does not place buy/sell orders.
- The one-year projection, when implemented, will be an estimate and not a guarantee of returns.

## Contribution Practice
Each team member should use their own GitHub account and make regular, meaningful commits for their actual work. Do not claim work that was not performed. Update the progress section as features are implemented and tested.

## References
- Python documentation Socket Programming
- C++ reference Classes, Destructors, String Streams, and Dynamic Memory Management
- Documentation for selected broker, market-data, and news interface
