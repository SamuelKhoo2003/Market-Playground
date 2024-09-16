import unittest
from collections import defaultdict, deque
from bisect import insort_left, insort_right

class OrderBook:
    def __init__(self):
        self.offer_books = defaultdict(deque)
        self.bid_books = defaultdict(deque)
        self.profit = 0

    def getExposure(self, books):
        exposure = 0
        for book in books.values():
            exposure += sum(a * b if owned else 0 for a, b, owned in book)
        return exposure

    def trade(self, records):
        self.profit = 0
        self.offer_books.clear()
        self.bid_books.clear()

        for record in records:
            record = record.split()
            share = record[0]

            i = 1
            while i < len(record):
                action = record[i]
                size = int(record[i+1])
                price = int(record[i+2])
                
                if action == 'BUY':
                    self._process_buy(share, size, price)
                elif action == 'SELL':
                    self._process_sell(share, size, price)
                elif action == 'OFFER':
                    self._process_offer(share, size, price)
                elif action == 'BID':
                    self._process_bid(share, size, price)

                i += 3

        long_exposure = self.getExposure(self.bid_books)
        short_exposure = self.getExposure(self.offer_books)
        return (self.profit, long_exposure, short_exposure)

    def _process_buy(self, share, size, price):
        while size > 0 and self.offer_books[share] and self.offer_books[share][0][0] <= price:
            offer_price, offer_size, isOwn = self.offer_books[share].popleft()
            minSize = min(size, offer_size)
            if not isOwn:
                self.profit += minSize * (price - offer_price)
            size -= minSize
            offer_size -= minSize
            if offer_size > 0:
                self.offer_books[share].appendleft((offer_price, offer_size, isOwn))
        if size > 0:
            insort_left(self.bid_books[share], (price, size, True))

    def _process_sell(self, share, size, price):
        while size > 0 and self.bid_books[share] and self.bid_books[share][-1][0] >= price:
            bid_price, bid_size, isOwn = self.bid_books[share].pop()
            minSize = min(size, bid_size)
            if not isOwn:
                self.profit += minSize * (bid_price - price)
            size -= minSize
            bid_size -= minSize
            if bid_size > 0:
                self.bid_books[share].append((bid_price, bid_size, isOwn))
        if size > 0:
            insort_right(self.offer_books[share], (price, size, True))

    def _process_offer(self, share, size, price):
        while size > 0 and self.bid_books[share] and self.bid_books[share][-1][0] >= price:
            bid_price, bid_size, isOwn = self.bid_books[share].pop()
            minSize = min(size, bid_size)
            if isOwn:
                self.profit += minSize * (bid_price - price)
            size -= minSize
            bid_size -= minSize
            if bid_size > 0:
                self.bid_books[share].append((bid_price, bid_size, isOwn))
        if size > 0:
            insort_right(self.offer_books[share], (price, size, False))

    def _process_bid(self, share, size, price):
        while size > 0 and self.offer_books[share] and self.offer_books[share][0][0] <= price:
            offer_price, offer_size, isOwn = self.offer_books[share].popleft()
            minSize = min(size, offer_size)
            if isOwn:
                self.profit += minSize * (price - offer_price)
            size -= minSize
            offer_size -= minSize
            if offer_size > 0:
                self.offer_books[share].appendleft((offer_price, offer_size, isOwn))
        if size > 0:
            insort_left(self.bid_books[share], (price, size, False))

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