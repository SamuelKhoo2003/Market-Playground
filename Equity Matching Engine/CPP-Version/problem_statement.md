# Matcher Engine

## Introduction

In the stock trading world, an **order book** is a record of all active orders for buying and selling shares, organized by priority. This matcher engine simulates the behavior of a stock trading system, where incoming buy and sell orders are processed and matched to ensure efficient trading. Orders are handled through various types such as **Market**, **Limit**, and **IOC (Immediate or Cancel)**, and the engine is responsible for ensuring the highest bid is matched with the lowest offer based on price and timestamp.

---

## Problem Description

You need to design a **Matcher Engine** to process and match buy and sell orders from an input stream. This engine supports several types of orders: Market, Limit, and IOC, and it should handle a variety of commands like placing new orders, amending existing ones, cancelling orders, and performing matches.

### Key Features:
- **Buy and Sell Orders:** Orders must be matched according to price and timestamp.
- **Matching Rules:**
  - The highest buy price is matched with the lowest sell price.
  - In case of a tie, the order placed earlier (based on the timestamp) is matched first (FIFO - First In First Out).
- **Order Types:**
  - **Market:** An order to buy/sell at the best available price.
  - **Limit:** An order to buy/sell at a specified price or better.
  - **IOC:** An order to be executed immediately at a specified price, with any unmatched portion being canceled.
- **Commands:**
  - **New Order** (N)
  - **Amend Order** (A)
  - **Cancel Order** (X)
  - **Match Orders** (M)
  
The system should output results of commands like acceptance or rejection of orders, amendments, cancellations, and matching results.

---

## Commands Description

1. **New Order (N)**
   - Places a new buy or sell order in the order book.
   - Command Format:
     ```
     N,<OrderID>,<Timestamp>,<Symbol>,<OrderType>,<Side>,<Price>,<Quantity>
     ```
   - Example:
     ```
     N,1,0000001,XYZ,L,B,104.53,100
     ```

   - **Output:**
     - On success: `<OrderID> - Accept`
     - On failure: `<OrderID> - Reject - 303 - Invalid order details`

2. **Amend Order (A)**
   - Updates the quantity or price of an existing order.
   - Command Format:
     ```
     A,<OrderID>,<Timestamp>,<Symbol>,<OrderType>,<Side>,<Price>,<Quantity>
     ```

   - **Output:**
     - On success: `<OrderID> - AmendAccept`
     - On failure: `<OrderID> - AmendReject - 101 - Invalid amendment details`
     - If order doesn't exist: `<OrderID> - AmendReject - 404 - Order does not exist`

3. **Cancel Order (X)**
   - Cancels an existing order.
   - Command Format:
     ```
     X,<OrderID>,<Timestamp>
     ```

   - **Output:**
     - On success: `<OrderID> - CancelAccept`
     - If order doesn't exist: `<OrderID> - CancelReject - 404 - Order does not exist`

4. **Match Orders (M)**
   - Matches existing buy and sell orders in the order book.
   - Command Format:
     ```
     M,<Timestamp>
     M,<Timestamp>,<Symbol>
     ```

   - **Output:**
     - Format:
       ```
       <Symbol>|<MatchedBuy>|<MatchedSell>
       ```
     - Example:
       ```
       XYZ|11,L,100,60.90|60.90,100,L,110
       ```

---

## Order Types

- **Market Order:**  
  - **Buy:** Purchase at the current lowest selling price.
  - **Sell:** Sell at the current highest buying price.

- **Limit Order:**  
  - **Buy:** Purchase at a specified price or lower.
  - **Sell:** Sell at a specified price or higher.

- **IOC (Immediate or Cancel) Order:**  
  - **Buy:** Attempt to purchase at a specified price, cancel any unmatched portion.
  - **Sell:** Attempt to sell at a specified price, cancel any unmatched portion.

---

## Example Commands and Outputs

### New Orders:

```
N,1,0000001,XYZ,L,B,104.53,100  
N,2,0000002,XYZ,L,B,100.30,100  
```
**Output:**

```
1 - Accept  
2 - Reject - 303 - Invalid order details
```

### Amend Orders:

```
A,1,0000001,XYZ,L,B,103.53,150
A,1,0000001,XYZ,L,S,103.53,150
```
**Output:**

```
1 - AmendAccept  
1 - AmendReject - 101 - Invalid amendment details
```

### Cancel Orders:

```
X,1,0000001
X,2,0000002
```
**Output:**

```
1 - CancelAccept  
2 - CancelReject - 404 - Order does not exist
```

### Matching Orders:

Consider the following new orders:

```
N,1,0000001,XYZ,L,B,60.90,200  
N,2,0000002,XYZ,L,S,60.90,100
N,3,0000003,XYZ,L,S,60.90,120
M,00010
```

**Output:**

```
XYZ|1,L,100,60.90|60.90,100,L,2
XYZ|1,L,100,60.90|60.90,100,L,3
```

---

## Matching Criteria

- **For Buy Orders:**
  - A buy order is matched with the lowest available sell order price.
  - Price must be less than or equal to the buy price.
  
- **For Sell Orders:**
  - A sell order is matched with the highest available buy order price.
  - Price must be greater than or equal to the sell price.
  
- Orders are matched based on:
  1. **Best Price**: Highest bid (buy) price with lowest ask (sell) price.
  2. **Timestamp (FIFO)**: Older orders are matched first when prices are tied.

---

## Summary

This matcher engine efficiently processes new buy/sell orders, amends orders, cancels orders, and matches orders based on price and timestamp. It handles different types of orders—Market, Limit, and IOC—ensuring fair and quick matching. The engine follows a simple command structure and outputs appropriate results based on the actions it processes.

---

### Potential Future Enhancements:
- Implement more complex order types (e.g., Stop Orders).
- Add support for multiple symbols in bulk matching commands.
- Introduce real-time price feeds to simulate dynamic trading environments.