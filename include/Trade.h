#ifndef TRADE_H
#define TRADE_H

struct Trade {
    long long buyOrderId;
    long long sellOrderId;
    double price;
    int quantity;
};

#endif