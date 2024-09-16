from collections import defaultdict, deque
from bisect import insort_left, insort_right
import unittest

class OrderBook:
    def __init__(self):
        self.offer_books = defaultdict(deque)
        self.bid_books = defaultdict(deque)
        self.profit = 0

    def getExposure(self, books):
        return sum(sum(a * b if owned else 0 for a, b, owned in book) for book in books.values())

    def trade(self, records):
        self.profit = 0
        self.offer_books.clear()
        self.bid_books.clear()

        for record in records:
            record = record.split()
            share = record[0]

            for i in range(1, len(record), 3):
                action, size, price = record[i], int(record[i+1]), int(record[i+2])
                self._process_action(share, action, size, price)

        return (self.profit, self.getExposure(self.bid_books), self.getExposure(self.offer_books))

    def _process_action(self, share, action, size, price):
        if action in ['BUY', 'BID']:
            self._process_order(share, size, price, self.offer_books, self.bid_books,
                                is_buy=True, is_own=(action == 'BUY'))
        elif action in ['SELL', 'OFFER']:
            self._process_order(share, size, price, self.bid_books, self.offer_books,
                                is_buy=False, is_own=(action == 'SELL'))

    def _process_order(self, share, size, price, opposite_book, same_book, is_buy, is_own):
        while size > 0 and opposite_book[share]:
            opposite_price, opposite_size, opposite_own = (opposite_book[share].popleft() if is_buy else opposite_book[share].pop())
            if (is_buy and opposite_price > price) or (not is_buy and opposite_price < price):
                opposite_book[share].append((opposite_price, opposite_size, opposite_own))
                break

            trade_size = min(size, opposite_size)
            if is_own != opposite_own:
                self.profit += trade_size * (price - opposite_price) * (1 if is_buy else -1)

            size -= trade_size
            opposite_size -= trade_size

            if opposite_size > 0:
                opposite_book[share].append((opposite_price, opposite_size, opposite_own))

        if size > 0:
            insort_fn = insort_left if is_buy else insort_right
            insort_fn(same_book[share], (price, size, is_own))

class TestOrderBook(unittest.TestCase):

    def setUp(self):
        self.order_book = OrderBook()

    def test_case_1(self):
        records = [
            "TINYCORP SELL 27 1",
            "MAVEN BID 5 20 OFFER 5 25",
            "MEDPHARMA BID 3 120 OFFER 7 150",
            "NEWFIRM BID 10 140 BID 7 150 OFFER 14 180",
            "TINYCORP BID 25 3 OFFER 25 6",
            "FASTAIR BID 21 65 OFFER 35 85",
            "FLYCARS BID 50 80 OFFER 100 90",
            "BIGBANK BID 200 13 OFFER 100 19",
            "REDCHIP BID 55 25 OFFER 80 30",
            "FASTAIR BUY 50 100",
            "CHEMCO SELL 100 67",
            "MAVEN BUY 5 30",
            "REDCHIP SELL 5 30",
            "NEWFIRM BUY 2 200",
            "MEDPHARMA BUY 2 150",
            "BIGBANK SELL 50 11",
            "FLYCARS BUY 200 100",
            "CHEMCO BID 1000 77 OFFER 500 88"
        ]
        expected_profit = 2740
        expected_long_exposure = 11500
        expected_short_exposure = 152

        result = self.order_book.trade(records)
        self.assertEqual(result, (expected_profit, expected_long_exposure, expected_short_exposure))

    def test_case_2(self):
        records = [
            "NEWFIRM BID 50 120",
            "NEWFIRM SELL 40 115",
            "TINYCORP BUY 10 100",
            "TINYCORP OFFER 10 105"
        ]
        expected_profit = 200
        expected_long_exposure = 1000
        expected_short_exposure = 0

        result = self.order_book.trade(records)
        self.assertEqual(result, (expected_profit, expected_long_exposure, expected_short_exposure))

    def test_case_3(self):
        records = [
            "FLYCARS BID 100 90",
            "FLYCARS SELL 100 85",
            "CHEMCO BUY 50 95",
            "CHEMCO OFFER 50 100",
            "BIGBANK BID 75 100",
            "BIGBANK SELL 50 90"
        ]
        expected_profit = 1000
        expected_long_exposure = 4750
        expected_short_exposure = 0

        result = self.order_book.trade(records)
        self.assertEqual(result, (expected_profit, expected_long_exposure, expected_short_exposure))

if __name__ == '__main__':
    unittest.main()