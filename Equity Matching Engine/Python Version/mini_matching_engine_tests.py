from mini_matching_engine import MatchingEngine, Order
from datetime import datetime

def print_separator():
    print("\n" + "="*50 + "\n")

def main():
    engine = MatchingEngine()
    order_id = 1

    print("Starting test run for the Enhanced Equity Order Matching Engine")
    print_separator()

    # Test 1: Add initial limit orders
    print("Test 1: Adding initial limit orders")
    initial_orders = [
        ('AAPL', 'L', 'B', 150.0, 100, 'GTC'),
        ('AAPL', 'L', 'S', 151.0, 100, 'GTC'),
        ('GOOGL', 'L', 'B', 2500.0, 10, 'GTC'),
        ('GOOGL', 'L', 'S', 2505.0, 10, 'GTC'),
        ('MSFT', 'L', 'B', 300.0, 50, 'GTC'),
        ('MSFT', 'L', 'S', 301.0, 50, 'GTC'),
    ]

    for symbol, order_type, side, price, quantity, time_in_force in initial_orders:
        order = Order(order_id, int(datetime.now().timestamp()), symbol, order_type, side, price, quantity, time_in_force)
        engine.add_order(order)
        order_id += 1
        print(f"Added order: {order}")

    print_separator()

    # Test 2: View order books
    print("Test 2: Viewing order books")
    for symbol in ['AAPL', 'GOOGL', 'MSFT']:
        print(engine.get_order_book(symbol))

    print_separator()

    # Test 3: Add matching limit order
    print("Test 3: Adding a matching limit order for AAPL")
    match_order = Order(order_id, int(datetime.now().timestamp()), 'AAPL', 'L', 'B', 151.0, 50, 'GTC')
    engine.add_order(match_order)
    order_id += 1
    print(f"Added order: {match_order}")
    print("\nUpdated AAPL order book:")
    print(engine.get_order_book('AAPL'))

    print_separator()

    # Test 4: Add market order
    print("Test 4: Adding a market order for GOOGL")
    market_order = Order(order_id, int(datetime.now().timestamp()), 'GOOGL', 'M', 'B', 0, 5, 'IOC')
    engine.add_order(market_order)
    order_id += 1
    print(f"Added order: {market_order}")
    print("\nUpdated GOOGL order book:")
    print(engine.get_order_book('GOOGL'))

    print_separator()

    # Test 5: Add IOC order
    print("Test 5: Adding an IOC order for MSFT")
    ioc_order = Order(order_id, int(datetime.now().timestamp()), 'MSFT', 'L', 'B', 301.0, 100, 'IOC')
    engine.add_order(ioc_order)
    order_id += 1
    print(f"Added order: {ioc_order}")
    print("\nUpdated MSFT order book:")
    print(engine.get_order_book('MSFT'))

    print_separator()

    # Test 6: View final order books
    print("Test 6: Viewing final order books")
    for symbol in ['AAPL', 'GOOGL', 'MSFT']:
        print(engine.get_order_book(symbol))

    print_separator()
    print("Test run completed.")

if __name__ == "__main__":
    main()