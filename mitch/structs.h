//
//  structs.h
//  mitch
//
//  Created by Mitchell Donahue on 7/29/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//

#ifndef structs_h
#define structs_h

#include <queue>
using namespace std;

struct BuyOrder {
    int timestamp = 0;
    int clientNum = 0;
    bool isBuy = false;
    int numShares = 0;
    int equityNum = 0;
    int price = 0;
    BuyOrder(int ts, int cn, bool buy, int en, int p, int ns) : timestamp(ts), clientNum(cn), isBuy(buy), equityNum(en), price(p), numShares(ns) {}
    bool operator <(const BuyOrder &b) const {
        if (price < b.price) {
            return false;
        } else if (price > b.price) {
            return true;
        }
        return false;
    };
};

struct Order {
    int timestamp = 0;
    int clientNum = 0;
    bool isBuy = false;
    int numShares = 0;
    int equityNum = 0;
    int price = 0;
    Order(int ts, int cn, bool buy, int en, int p, int ns) : timestamp(ts), clientNum(cn), isBuy(buy), equityNum(en), price(p), numShares(ns) {}
//    bool operator < (Order a) const {
//        if (a.price > price) {
//            return true;
//        } else if (a.price < price) {
//            return false;
//        }
//        if (a.timestamp > timestamp) {
//            return true;
//        } else if (a.timestamp < timestamp) {
//            return false;
//        }
//        return false;
//    }
};

class ToBuyCompare {
public:
    bool operator() (Order a, Order b) {
        if (a.price > b.price) {
            return false;
        } else if (a.price < b.price) {
            return true;
        }
        if (a.timestamp > b.timestamp) {
            return false;
        } else if (a.timestamp < b.timestamp) {
            return true;
        }
        return true;
    }
};

class ToSellCompare {
public:
    bool operator() (Order a, Order b) {
        if (a.price > b.price) {
            return true;
        } else if (a.price < b.price) {
            return false;
        }
        if (a.timestamp > b.timestamp) {
            return true;
        } else if (a.timestamp < b.timestamp) {
            return false;
        }
        return false;
    }
};

struct StockOrders {
    priority_queue<Order, vector<Order>, ToSellCompare> toSell;
    priority_queue<Order, vector<Order>, ToBuyCompare> toBuy;
};

struct ClientInfo {
    int numBought = 0;
    int numSold = 0;
    int netVal = 0;
};

struct TimeTraveler {
    int lowestPriceSeen = 0;
    int lowestPriceTime = 0;
    int bestProfit = 0;
    int buyTime = -1;
    int sellTime = -1;
};

#endif /* structs_h */
