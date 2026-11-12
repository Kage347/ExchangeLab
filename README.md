# ExchangeLab

A C++20 learning project exploring limit orders, partial fills and trade records. The current version is a small, runnable exercise developed with guidance. It processes manually selected buy/sell pairs and checks the results with assertions.

## What works now

- An `Order` stores its ID, side, limit price, submitted quantity and remaining quantity.
- Separate functions check positive order terms and whether a buy limit crosses a sell limit.
- A buy and a resting sell can trade at the resting seller's price.
- Fill size is limited by both orders' remaining quantities, which are updated after each trade.
- A trade log records the buyer, seller, price and quantity.
- The executable contains 23 assertions covering invalid terms, equal/crossing/non-crossing prices, consecutive partial fills and retrying an exhausted buyer.

Prices are integer ticks and quantities are whole shares. In the examples, one tick represents one penny.

## Run the exercise

Requires a C++20 compiler and the standard library. From the repository root:

```sh
mkdir -p build
c++ -std=c++20 -Wall -Wextra -Wpedantic exercises/order_terms.cpp -o build/order_terms
./build/order_terms
```

Successful output:

```text
Order term checks passed.
```

The checks use `assert`, so run the command as shown without defining `NDEBUG`.

## A consecutive-fill example

Buyer B5 wants 5 shares with a limit of 103 ticks. A resting seller offers 2 shares at 101, followed by another offering 4 at 102.

| Step | Trade | B5 remaining | Seller remaining |
| --- | --- | ---: | ---: |
| Match S3 | 2 shares at 101 | 3 | S3: 0 |
| Match S5 | 3 shares at 102 | 0 | S5: 1 |
| Retry B5 against S5 | No trade | 0 | S5: 1 |

An earlier version reused the submitted quantity when sizing each fill and overfilled the buyer on the second match. During guided development, the exercise was changed to track remaining quantities, update both orders after a fill and check the exhausted-order retry.

## Current scope

This is an early exercise, not a complete exchange or matching engine. The caller supplies valid buy/sell pairs in the chosen order. There is no automatic order-book search, price-time queue, cancellation handling, duplicate-ID enforcement, market-data feed or concurrent processing. Input validation is currently tested separately from trade recording.

## Next steps

- Reconstruct and explain the current logic independently.
- Add an ask-side price map and check best-price selection and arrival order at equal prices.
- Extend the tests as order-book and cancellation behaviour is implemented.

