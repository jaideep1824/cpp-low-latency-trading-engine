#ifndef ORDER_H
#define ORDER_H

enum class OrderSide {
    BUY,
    SELL
};

struct Order {
    long long id;
    OrderSide side;
    double price;
    int quantity;
};

#endif