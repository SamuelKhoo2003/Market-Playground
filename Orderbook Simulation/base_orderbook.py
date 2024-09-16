from collections import defaultdict, deque
from bisect import insort_left, insort_right

# Function to calculate the exposure
def getExposure(books: dict) -> int:
    exposure = 0
    for book in books.values():
        exposure += sum(a * b if owned else 0 for a, b, owned in book)
    return exposure

# Main trading function
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
            size = int(record[i+1])
            price = int(record[i+2])
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
                while size > 0 and bid_books[share] and bid_books[share][0][0] >= price:
                    bid_price, bid_size, isOwn = bid_books[share].pop()
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
                while size > 0 and bid_books[share] and bid_books[share][0][0] >= price:
                    bid_price, bid_size, isOwn = bid_books[share].pop()
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

    long_exposure = getExposure(bid_books)
    short_exposure = getExposure(offer_books)
    return (profit, long_exposure, short_exposure)

# Test case function
def run_test_case():
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

    result = trade(records)
    
    print(f"Expected: Profit = {expected_profit}, Long Exposure = {expected_long_exposure}, Short Exposure = {expected_short_exposure}")
    print(f"Result: Profit = {result[0]}, Long Exposure = {result[1]}, Short Exposure = {result[2]}")
    
    assert result == (expected_profit, expected_long_exposure, expected_short_exposure), "Test case failed"
    print("Test case passed!")

if __name__ == "__main__":
    run_test_case()
