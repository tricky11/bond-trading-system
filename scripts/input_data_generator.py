import argparse
import random
import uuid

product_ids = ["9128283H1", "9128283L2", "912828M80", "9128283J7", "9128283F5", "912810RZ3"]


def main():
    parser = argparse.ArgumentParser(
        description='Generates input data for use in BondTradingSystem. '
                    'By default, this program generates all 4 types of data. '
                    'Use optional arguments to generate only 1 specific dataset.')
    parser.add_argument('--trades', help='Generate only trades data.', action='store_true')
    parser.add_argument('--prices', help='Generate only prices data.', action='store_true')
    parser.add_argument('--inquiries', help='Generate only inquiries data.', action='store_true')
    parser.add_argument('--marketdata', help='Generate only market(orderbook) data.', action='store_true')
    args = parser.parse_args()
    if args.trades:
        generate_trades()
    elif args.inquiries:
        generate_inquiries()
    elif args.prices:
        generate_prices()
    elif args.marketdata:
        generate_marketdata()
    else:
        generate_trades()
        generate_inquiries()
        generate_prices()
        generate_marketdata()


def generate_trades():
    volumes = [1000000, 2000000, 3000000, 4000000, 5000000]
    volume_state = 0

    prices = [99.0, 100.0]
    price_state = 0

    side = 0

    books = ["TRSY1", "TRSY2", "TRSY3"]
    book_state = 0

    input_file = open("../input/trades.csv", "w")
    input_file.write("ProductId,TradeId,Price,Book,Quantity,Side\n")
    for product_id in product_ids:
        for i in range(10):
            trade_id = str(uuid.uuid4()).split('-')[0]
            input_file.write(product_id + "," +
                             trade_id + "," +
                             str(prices[price_state]) + "," +
                             books[book_state] + "," +
                             str(volumes[volume_state]) + "," +
                             str(side) + "\n")
            volume_state = (volume_state + 1) % len(volumes)
            price_state = (price_state + 1) % len(prices)
            book_state = (book_state + 1) % len(books)
            side = (side + 1) % 2
    input_file.close()
    print("Generated trades.csv")


def generate_inquiries():
    volumes = [1000000, 2000000, 3000000, 4000000, 5000000]
    volume_state = 0

    side = 0

    input_file = open("../input/inquiries.csv", "w")
    input_file.write("ProductId,InquiryId,Side,Quantity\n")
    for product_id in product_ids:
        for i in range(10):
            inquiry_id = str(uuid.uuid4()).split('-')[0]
            input_file.write(product_id + "," +
                             inquiry_id + "," +
                             str(side) + "," +
                             str(volumes[volume_state]) + "\n")
            volume_state = (volume_state + 1) % len(volumes)
            side = (side + 1) % 2
    input_file.close()
    print("Generated inquiries.csv")


def generate_prices():
    input_file = open("../input/prices.csv", "w")
    data = "ProductId,Mid,Spread\n"
    print "Buffering prices data.\n" \
          "WARNING: This takes a long time. Do not stop this process in between.\n" \
          "Nothing will be written to file until it completes."
    for i, product_id in enumerate(product_ids):
        for j in xrange(1000000):
            if j % 100000 == 0:
                print("prices.csv in progress: {0:.2f}%".format(
                    100.0 * (1000000 * i + j) / (len(product_ids) * 1000000)))
            data += (product_id + "," +
                     get_random_fractional_price(99, 101) + "," +
                     get_random_spread() + "\n")
    print("Writing prices.csv")
    input_file.write(data)
    input_file.close()
    print("Generated prices.csv")


def generate_marketdata():
    volumes = [10000000, 20000000, 30000000, 40000000, 50000000]
    volume_state = 0

    spread = 2
    spread_increment = 2

    input_file = open("../input/marketdata.csv", "w")
    data = "ProductId,BidPrice1,BidVolume1,BidPrice2,BidVolume2,BidPrice3,BidVolume3,BidPrice4,BidVolume4,BidPrice5,BidVolume5,OfferPrice1,OfferVolume1,OfferPrice2,OfferVolume2,OfferPrice3,OfferVolume3,OfferPrice4,OfferVolume4,OfferPrice5,OfferVolume5\n"

    print "Buffering market data.\n" \
          "WARNING: This takes a long time. Do not stop this process in between.\n" \
          "Nothing will be written to file until it completes."

    for i, product_id in enumerate(product_ids):
        for j in range(1000000):
            if j % 100000 == 0:
                print("marketdata.csv in progress: {0:.2f}%".format(
                    100.0 * (1000000 * i + j) / (len(product_ids) * 1000000)))
            data += (product_id)
            mid = random.randint(99 * 256, 101 * 256)  # value multiplied by 256
            for k in range(5):
                data += "," + get_fractional_representation(mid - spread / 2 - k) + "," + str(volumes[volume_state])
                volume_state = (volume_state + 1) % len(volumes)
            for k in range(5):
                data += "," + get_fractional_representation(mid + spread / 2 + k) + "," + str(volumes[volume_state])
                volume_state = (volume_state + 1) % len(volumes)
            data += "\n"
            spread += spread_increment
            if (spread == 8) and (spread_increment == 2):
                spread_increment = -2
            if (spread == 2) and (spread_increment == -2):
                spread_increment = 2

    print("Writing marketdata.csv")
    input_file.write(data)
    input_file.close()
    print("Generated marketdata.csv")


def get_random_fractional_price(low, high):
    first_part = random.randint(0, 31)
    second_part = random.randint(0, 7)
    return str(random.randint(low, high - 1)) + "-" + (str(first_part) if first_part > 9 else "0" + str(first_part)) + (
        "+" if second_part == 4 else str(second_part))


def get_random_spread():
    return "00-" + "23+"[random.randint(0, 2)]


def get_fractional_representation(increment_number):
    first_part = increment_number / 256
    remainder = increment_number % 256
    second_part = remainder / 8
    third_part = remainder % 8
    return str(first_part) + "-" + \
           (str(second_part) if second_part > 9 else "0" + str(second_part)) + \
           ("+" if third_part == 4 else str(third_part))


if __name__ == '__main__':
    main()
