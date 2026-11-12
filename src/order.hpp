#pragma once

#include <string>

enum class Side { Buy, Sell };

struct Order {
    std::string id;
    // Add the side, limit price in ticks and submitted quantity in shares.
    Side side;
    int price_ticks;
    int quantity;
    int remaining_quantity = quantity;
};
