from collections import defaultdict, deque
from itertools import insort_left, insort_right

def getExposure(books: dict) -> int:
    exposure = 0
    for book in books:
        exposure += sum(a * b for a, b, owned in book)
    return exposure

def trade(records: list[str]) -> tuple[int, int, int]:
    profit = 0
    long_exposure, short_exposure = 0, 0

    offer_books = defaultdict(deque)
    bid_books = defaultdict(deque)

    for record in records:
        record = record.split()
        share = record[0]

        i = 1
        while i < len(record):
            action = record[i]
            size = record[i+1]
            price = record[i+2]
            if action == 'BUY':
                while size > 0 and offer_books[share] and offer_books[share][0][0] <= price:
                    offer_price, offer_size, isOwn = offer_books[share].popleft()
                    minSize = min(size, offer_size)
                    if not isOwn:
                        profit += minSize * (price - offer_price)
                    size -= minSize
                    offer_size -= minSize
                    if offer_size > 0:
                        offer_books[share].appendleft((offer_price, offer_size, isOwn))
                if size > 0:
                    insort_left(bid_books[share], (price, size, True))
            elif action == 'SELL':
                while size > 0 and bid_books[share] and bid_books[share][0][-1] >= price:
                    bid_price, bid_size, isOwn = bid_books.pop()
                    minSize = min(size, bid_size)
                    if not isOwn:
                        profit += minSize * (bid_price - price)
                    size -= minSize
                    bid_size -= minSize
                    if bid_size > 0:
                        bid_books[share].append((bid_price, bid_size, isOwn))
                if size > 0:
                    insort_right(offer_books[share], (price, size, True))
            elif action == 'OFFER':
                while size > 0 and bid_books[share] and bid_books[share][0][-1] >= price:
                    bid_price, bid_size, isOwn = bid_books.pop()
                    minSize = min(size, bid_size)
                    if isOwn:
                        profit += minSize * (bid_price - price)
                    size -= minSize
                    bid_size -= minSize
                    if bid_size > 0:
                        bid_books[share].append((bid_price, bid_size, isOwn))
                if size > 0:
                    insort_right(offer_books[share], (price, size, False))
            elif action == 'BID':
                while size > 0 and offer_books[share] and offer_books[share][0][0] <= price:
                    offer_price, offer_size, isOwn = offer_books[share].popleft()
                    minSize = min(size, offer_size)
                    if isOwn:
                        profit += minSize * (price - offer_price)
                    size -= minSize
                    offer_size -= minSize
                    if offer_size > 0:
                        offer_books[share].appendleft((offer_price, offer_size, isOwn))
                if size > 0:
                    insort_left(bid_books[share], (price, size, False))

            i += 3

    long_exposure = getExposure(offer_books)
    short_exposure = getExposure(bid_books)
    return (profit, long_exposure, short_exposure)
