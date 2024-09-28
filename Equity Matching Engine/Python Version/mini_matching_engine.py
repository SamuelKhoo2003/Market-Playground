from typing import List, Tuple, Dict
from datetime import datetime
import heapq

class Order:
    def __init__(self, order_id: int, timestamp: int, symbol: str, order_type: str, side: str, price: float, quantity: int, time_in_force: str = 'GTC'):
        self.order_id = order_id
        self.timestamp = timestamp
        self.symbol = symbol
        self.order_type = order_type
        self.side = side
        self.price = price
        self.quantity = quantity
        self.time_in_force = time_in_force

    def __repr__(self):
        return f"Order({self.order_id}, {self.symbol}, {self.side}, {self.price}, {self.quantity}, {self.time_in_force})"

    def __lt__(self, other):
        if self.price == other.price:
            return self.timestamp < other.timestamp
        return self.price < other.price if self.side == 'S' else self.price > other.price

class OrderBook:
    def __init__(self):
        self.buy_orders = {}
        self.sell_orders = {}

    def add_order(self, order: Order):
        if order.side == 'B':
            heapq.heappush(self.buy_orders.setdefault(order.symbol, []), (-order.price, order.timestamp, order))
        else:
            heapq.heappush(self.sell_orders.setdefault(order.symbol, []), (order.price, order.timestamp, order))

    def remove_order(self, order: Order):
        if order.side == 'B':
            self.buy_orders[order.symbol] = [o for o in self.buy_orders[order.symbol] if o[2].order_id != order.order_id]
            heapq.heapify(self.buy_orders[order.symbol])
        else:
            self.sell_orders[order.symbol] = [o for o in self.sell_orders[order.symbol] if o[2].order_id != order.order_id]
            heapq.heapify(self.sell_orders[order.symbol])

    def get_best_buy(self, symbol: str):
        return -self.buy_orders[symbol][0][0] if symbol in self.buy_orders and self.buy_orders[symbol] else None

    def get_best_sell(self, symbol: str):
        return self.sell_orders[symbol][0][0] if symbol in self.sell_orders and self.sell_orders[symbol] else None

    def get_order_book_str(self, symbol: str):
        buy_orders = sorted([(o[2].price, o[2].quantity) for o in self.buy_orders.get(symbol, [])], reverse=True)
        sell_orders = sorted([(o[2].price, o[2].quantity) for o in self.sell_orders.get(symbol, [])])
        
        book_str = f"Order Book for {symbol}:\n"
        book_str += "Buy Orders:\n"
        for price, quantity in buy_orders[:5]:  # Show top 5 levels
            book_str += f"  {price}: {quantity}\n"
        book_str += "Sell Orders:\n"
        for price, quantity in sell_orders[:5]:  # Show top 5 levels
            book_str += f"  {price}: {quantity}\n"
        return book_str

class MatchingEngine:
    def __init__(self):
        self.order_book = OrderBook()
        self.matchbook: Dict[str, List[Tuple[Order, Order]]] = {}

    def add_order(self, order: Order):
        if order.order_type == 'M':
            self.process_market_order(order)
        elif order.order_type == 'L':
            self.process_limit_order(order)
        
        if order.quantity > 0 and order.time_in_force != 'IOC':
            self.order_book.add_order(order)

    def process_market_order(self, order: Order):
        opposite_side = 'S' if order.side == 'B' else 'B'
        opposite_orders = self.order_book.sell_orders.get(order.symbol, []) if order.side == 'B' else self.order_book.buy_orders.get(order.symbol, [])
        
        while order.quantity > 0 and opposite_orders:
            best_order = heapq.heappop(opposite_orders)[2]
            matched_quantity = min(order.quantity, best_order.quantity)
            self.match_orders(order, best_order, matched_quantity)
            
            if best_order.quantity > 0:
                heapq.heappush(opposite_orders, (best_order.price, best_order.timestamp, best_order))

    def process_limit_order(self, order: Order):
        opposite_side = 'S' if order.side == 'B' else 'B'
        opposite_orders = self.order_book.sell_orders.get(order.symbol, []) if order.side == 'B' else self.order_book.buy_orders.get(order.symbol, [])
        
        while order.quantity > 0 and opposite_orders:
            best_order = opposite_orders[0][2]
            if (order.side == 'B' and order.price >= best_order.price) or (order.side == 'S' and order.price <= best_order.price):
                heapq.heappop(opposite_orders)
                matched_quantity = min(order.quantity, best_order.quantity)
                self.match_orders(order, best_order, matched_quantity)
                
                if best_order.quantity > 0:
                    heapq.heappush(opposite_orders, (best_order.price, best_order.timestamp, best_order))
            else:
                break

    def match_orders(self, order1: Order, order2: Order, quantity: int):
        price = order2.price  # Use the price of the resting order
        order1.quantity -= quantity
        order2.quantity -= quantity
        
        if order2.quantity == 0:
            self.order_book.remove_order(order2)
        
        self.matchbook.setdefault(order1.symbol, []).append((order1, order2, quantity, price))
        print(f"Matched: {order1.symbol} - {quantity} @ {price}")

    def get_order_book(self, symbol: str):
        return self.order_book.get_order_book_str(symbol)

def main():
    engine = MatchingEngine()
    order_id = 1

    # Add some initial orders
    initial_orders = [
        ('AAPL', 'L', 'B', 150.0, 100),
        ('AAPL', 'L', 'S', 151.0, 100),
        ('GOOGL', 'L', 'B', 2500.0, 10),
        ('GOOGL', 'L', 'S', 2505.0, 10),
        ('MSFT', 'L', 'B', 300.0, 50),
        ('MSFT', 'L', 'S', 301.0, 50),
    ]

    for symbol, order_type, side, price, quantity in initial_orders:
        order = Order(order_id, int(datetime.now().timestamp()), symbol, order_type, side, price, quantity)
        engine.add_order(order)
        order_id += 1

    while True:
        command = input("Enter command (e.g., 'ADD,AAPL,L,B,150.5,10,GTC' or 'BOOK,AAPL' or 'EXIT'): ")
        if command.upper() == 'EXIT':
            break

        parts = command.split(',')
        if parts[0].upper() == 'ADD':
            if len(parts) < 7:
                print("Invalid ADD command. Format: ADD,SYMBOL,TYPE,SIDE,PRICE,QUANTITY,TIME_IN_FORCE")
                continue
            symbol, order_type, side, price, quantity, time_in_force = parts[1:7]
            order = Order(order_id, int(datetime.now().timestamp()), symbol, order_type, side, float(price), int(quantity), time_in_force)
            engine.add_order(order)
            order_id += 1
            print(f"Added order: {order}")
        elif parts[0].upper() == 'BOOK':
            if len(parts) < 2:
                print("Invalid BOOK command. Format: BOOK,SYMBOL")
                continue
            symbol = parts[1]
            print(engine.get_order_book(symbol))
        else:
            print("Invalid command. Use ADD, BOOK, or EXIT.")

if __name__ == "__main__":
    main()