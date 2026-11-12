#include <cassert>
#include <iostream>
#include <algorithm>
#include "../src/order.hpp"
#include <vector>

bool has_invalid_terms(int price_ticks, int quantity) {
    if (price_ticks <= 0 || quantity <= 0) {
        return true;
    }
    return false;
}

bool prices_cross(int buy_limit_ticks, int sell_limit_ticks) {
    // Return the result of comparing the two prices.
    if (buy_limit_ticks >= sell_limit_ticks) {
        return true;
    }
    return false;
}

int executable_quantity(const Order& buy, const Order& sell) {
    if (!prices_cross(buy.price_ticks, sell.price_ticks)) {
        return 0;
    }

    return std::min(buy.remaining_quantity, sell.remaining_quantity);
}

struct Trade {
    std::string buy_id;
    std::string sell_id;
    int price_ticks;
    int quantity;
};

void record_trade(Order& buy, Order& resting_sell,
                  std::vector<Trade>& trades) {
    int fill_quantity = executable_quantity(buy, resting_sell);

    if (fill_quantity > 0) {
        Trade trade{buy.id, resting_sell.id, resting_sell.price_ticks, fill_quantity};
        trades.push_back(trade);
        buy.remaining_quantity -= fill_quantity;
        resting_sell.remaining_quantity -= fill_quantity;
    }
}

int main() {

    Order b5{"B5", Side::Buy, 103, 5};
    Order s3{"S3", Side::Sell, 101, 2};

    Order s4("S4", Side::Sell, 104, 2);

    std::vector<Trade> trades;
    assert(trades.empty());

    record_trade(b5, s4, trades);
    assert(trades.empty());

    assert(executable_quantity(b5, s4) == 0);

    
    assert(prices_cross(b5.price_ticks, s3.price_ticks));
    int fill_quantity = executable_quantity(b5, s3);
    assert(fill_quantity == 2);


    record_trade(b5, s3, trades);
    assert(trades.size() == 1);
    assert(trades.front().price_ticks == 101);
    assert(trades.front().quantity == 2);
    assert(b5.remaining_quantity == 3);
    assert(s3.remaining_quantity == 0);

    Order s5{"S5", Side::Sell, 102, 4};
    record_trade(b5, s5, trades);
    assert(trades.size() == 2);
    assert(trades.back().price_ticks == 102);
    assert(trades.back().quantity == 3);
    assert(b5.remaining_quantity == 0);
    assert(s5.remaining_quantity == 1);

    record_trade(b5, s5, trades);
    assert(trades.size() == 2);
    assert(s5.remaining_quantity == 1);


    assert(has_invalid_terms(100, 0));
    assert(!has_invalid_terms(100, 2));
    assert(has_invalid_terms(0, 2));
    assert(prices_cross(101, 101));
    assert(prices_cross(103, 101));
    assert(!prices_cross(100, 101));
    std::cout << "Order term checks passed.\n";


}
