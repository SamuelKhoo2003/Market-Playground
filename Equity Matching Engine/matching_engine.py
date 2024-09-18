class Order:
    def __init__(self, order_id=None, timestamp=None, symbol=None, side=None, order_type=None, price=None, quantity=None):
        self.OrderID = order_id
        self.Timestamp = timestamp
        self.Symbol = symbol
        self.Side = side  # 'B' for Buy, 'S' for Sell
        self.OrderType = order_type  # 'M' for Market, 'L' for Limit
        self.Price = price
        self.Quantity = quantity

    # Getter methods
    def get_id(self):
        return self.OrderID

    def get_timestamp(self):
        return self.Timestamp

    def get_quantity(self):
        return self.Quantity

    def get_price(self):
        return self.Price

    def get_side(self):
        return self.Side

    def get_type(self):
        return self.OrderType

    def get_symbol(self):
        return self.Symbol

    # Setter methods
    def set_price(self, price):
        self.Price = price

    def set_quantity(self, quantity):
        self.Quantity = quantity


# List of all orders
OrderBook = []

# Dictionary where key is the symbol, value is a list of matched (buy, sell) order pairs
Matchbook = {}

### Helper functions ###
def is_char(s: str) -> bool:
    """Check if the string is a valid single character from the set {B, S, M, L, I}."""
    return s in {'B', 'S', 'M', 'L', 'I'}


def is_int(s: str) -> bool:
    """Check if a string represents a valid integer."""
    if not s or not s[0].isdigit():  # Empty or doesn't start with a digit
        return False
    try:
        int(s)
        return True
    except ValueError:
        return False


def is_float(s: str) -> bool:
    """Check if a string represents a valid float."""
    try:
        float(s)
        return True
    except ValueError:
        return False


def comp(order1, order2) -> bool:
    """Compare two orders based on their symbols."""
    return order1.get_symbol() <= order2.get_symbol()


def to_stringp(a_value: float) -> str:
    """Convert a float to a string with two decimal places."""
    return f"{a_value:.2f}"


def split_order_str(s: str) -> list:
    """Split a string by a comma delimiter into components."""
    return s.split(',')


# Example usage:
# Adding an order to the OrderBook
order1 = Order(order_id=1, timestamp=1234567890, symbol="AAPL", side='B', order_type='L', price=150.0, quantity=10)
order2 = Order(order_id=2, timestamp=1234567891, symbol="AAPL", side='S', order_type='L', price=150.0, quantity=10)

OrderBook.append(order1)
OrderBook.append(order2)

# Adding matched orders to the Matchbook
symbol = order1.get_symbol()
if symbol not in Matchbook:
    Matchbook[symbol] = []

Matchbook[symbol].append((order1, order2))

# Printing the orders in the OrderBook
for order in OrderBook:
    print(f"Order ID: {order.get_id()}, Symbol: {order.get_symbol()}, Side: {order.get_side()}, Price: {order.get_price()}, Quantity: {order.get_quantity()}")

# Printing matched orders
for symbol, matches in Matchbook.items():
    print(f"Symbol: {symbol}")
    for buy_order, sell_order in matches:
        print(f"Buy Order ID: {buy_order.get_id()} matched with Sell Order ID: {sell_order.get_id()}")
