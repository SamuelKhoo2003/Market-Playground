# Matching Engine Solutions

This repository contains three Python implementations (`base_orderbook.py`, `class_orderbook.py`, and `optimised_orderbook.py`) for a matching engine, simulating the activity of trading shares in a market. The solutions progressively improve upon the design by introducing code modularity, class-based structure, and optimizations.

## Problem Summary

You are tasked with processing a series of records representing a day of activity of trading shares between two sides: **Buyers** and **Sellers**. The format of each record is as follows:

```
[Share] [Action_1] [Size_1] [Price_1] [Action_2] [Size_2] [Price_2] [...]
```

### Definitions:
- **Share**: The company whose shares are being traded.
- **Action**: One of four possible strings: `BUY`, `SELL`, `BID`, `OFFER`.
    - `BUY`/`SELL` represent orders from your own side.
    - `BID`/`OFFER` represent orders from other participants in the market.
- **Size**: The number of shares being traded.
- **Price**: The price per share for the order.

### Requirements:
1. **Matching Logic**:
    - Orders should be matched as soon as possible. For a `BUY`, you want to match it with an existing `OFFER` where the price is **equal to or lower**. For a `SELL`, match with a `BID` where the price is **equal to or higher**.
    - Orders that cannot be matched are stored and available for future matching.
2. **Partial Matches**: If an order is partially matched, the unmatched portion remains in the order book.
3. **Best Price First**: Orders should be matched based on the best price, with `BID`s being prioritized by the **highest** price and `OFFER`s by the **lowest** price.
4. **Profit Calculation**: Profit is calculated when a `BUY` or `SELL` from your side is matched with another order. Profit is the difference between the matched prices.
5. **Exposure**: Long and short exposure is calculated based on open bids and offers that are yet to be matched.

## File Structure

### `base_orderbook.py`

This file contains the initial version of the order book matching engine. The code is written in a basic procedural style. It:
- Handles matching orders from `BUY` and `SELL` actions with `BID` and `OFFER`.
- Supports partial order matching and calculates profit based on the difference in matched prices.
- Calculates long and short exposures after processing all records.

**Limitations**: The design is simple but contains duplicated logic for matching and lacks a clear object-oriented structure. It may be inefficient for large datasets.

### `class_orderbook.py`

This version refactors the order book implementation into a class-based structure. It introduces the following improvements:
- **Modularity**: Logic for managing bids, offers, matching, and profit calculation is encapsulated in an `OrderBook` class.
- **Cleaner Design**: The class structure reduces code duplication, making the code more maintainable and scalable.
- **Better Abstraction**: By representing orders and the order book as objects, the design becomes more intuitive and easy to extend.

### `optimised_orderbook.py`

This file builds upon the `class_orderbook.py` and includes performance optimizations:
- **Optimized Matching**: Uses efficient data structures (like `deque` and `insort_left/right` from `bisect`) to speed up the matching of orders.
- **Improved Profit Calculation**: Profit is calculated dynamically as orders are processed.
- **Efficient Exposure Calculation**: The calculation of long and short exposure is optimized to handle larger datasets more effectively.

This implementation is designed for use in competitive programming environments or real-time matching systems, providing both efficiency and clarity.

## Test Cases

Each file has its own set of unit tests that verify the correctness of the implementation across a variety of scenarios, including:
- Basic matching of orders.
- Handling partial matches.
- Complex sequences of multiple orders.
- Correct calculation of profit and exposure.

**Example Test Case:**
```python
records = [
    "MARKETMAKERONE TESTER BUY 10 25 BID 20 24",
    "MARKETMAKERTWO SELL 5 15",
    "MARKETMAKERTWO OFFER 7 17"
]

# Expected Output:
# Profit: X (calculated based on match)
# Long Exposure: Y (based on remaining bids)
# Short Exposure: Z (based on remaining offers)
```


