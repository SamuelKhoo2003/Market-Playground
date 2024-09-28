# Equity Matching Engine

## Overview

This project implements a simple yet powerful equity order matching engine in Python. It simulates the core functionality of a stock exchange's order matching system, supporting multiple stocks, various order types, and a price-time priority matching algorithm.

## Files

1. `equity_order_matching_engine.py`: The main implementation of the order matching engine.
2. `test_order_matching_engine.py`: A test script to demonstrate and verify the functionality of the matching engine.

## Features

- Support for multiple stocks (e.g., AAPL, GOOGL, MSFT)
- Order types: Market (M) and Limit (L)
- Time-in-force options: Good-Till-Cancelled (GTC) and Immediate-or-Cancel (IOC)
- Price-time priority matching algorithm
- Basic order book visualization
- Simple command-line interface for manual interaction

## Equity Matching Engine (`equity_order_matching_engine.py`)

### Key Components

1. `Order` class: Represents individual orders with attributes such as order ID, timestamp, symbol, type, side, price, quantity, and time-in-force.

2. `OrderBook` class: Manages the order book for each stock, including methods for adding, removing, and retrieving orders.

3. `MatchingEngine` class: Implements the core matching logic, processes incoming orders, and manages the order books for all stocks.

### Main Functionality

- Add new orders (market or limit)
- Process market orders
- Process limit orders
- Match orders based on price-time priority
- Maintain and display order books for each stock

## Test Script (`test_order_matching_engine.py`)

The test script demonstrates the functionality of the matching engine through a series of scenarios:

1. Adding initial limit orders for multiple stocks
2. Viewing order books
3. Adding a matching limit order
4. Processing a market order
5. Handling an Immediate-or-Cancel (IOC) order
6. Displaying final order books

## Usage

### Running the Matching Engine

1. Ensure you have Python 3.x installed.
2. Save `equity_order_matching_engine.py` in your working directory.
3. Run the script:
   ```
   python equity_order_matching_engine.py
   ```
4. Use the command-line interface to interact with the engine:
   - Add orders: `ADD,SYMBOL,TYPE,SIDE,PRICE,QUANTITY,TIME_IN_FORCE`
     Example: `ADD,AAPL,L,B,150.5,10,GTC`
   - View order book: `BOOK,SYMBOL`
     Example: `BOOK,AAPL`
   - Exit: `EXIT`

### Running the Test Script

1. Ensure both `equity_order_matching_engine.py` and `test_order_matching_engine.py` are in the same directory.
2. Run the test script:
   ```
   python test_order_matching_engine.py
   ```
3. Review the output to verify the engine's functionality.

## Extending the Project

To extend or customize the matching engine:

1. Add support for additional order types (e.g., stop orders, limit-on-close)
2. Implement more sophisticated matching algorithms
3. Add persistence to store order data
4. Create a graphical user interface (GUI) for easier interaction
5. Implement real-time data feeds for stock prices

## Contributing

Contributions to improve the matching engine or add new features are welcome. Please submit pull requests or open issues to discuss potential changes.
