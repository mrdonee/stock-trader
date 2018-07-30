//
//  marketInput.h
//  mitch
//
//  Created by Mitchell Donahue on 7/27/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//

#ifndef marketInput_h
#define marketInput_h

#include <iostream>
#include <vector>
#include <queue>
#include "marketInput.h"
using namespace std;

struct Order {
    int timestamp = 0;
    int clientNum = 0;
    bool isBuy = false;
    int numShares = 0;
    int equityNum = 0;
    int price = 0;
    Order(int ts, int cn, bool buy, int en, int p, int ns) : timestamp(ts), clientNum(cn), isBuy(buy), equityNum(en), price(p), numShares(ns) {}
    Order(const Order & d) :
    timestamp(d.timestamp), clientNum(d.clientNum), isBuy(d.isBuy), numShares(d.numShares), equityNum(d.equityNum), price(d.price) {
        
    }
};

class ToBuyCompare {
public:
    bool operator() (Order a, Order b) { //TODO
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

class MedianTracker {
    priority_queue<int, vector<int>, greater<>> topHalf;
    priority_queue<int> bottomHalf;
public:
    int getMedian() {
        if (topHalf.size() == bottomHalf.size() + 1) {
            return topHalf.top();
        } else if (topHalf.size() == bottomHalf.size()) {
            if (!size()) {
                cerr << "Trying to get median from empty PQs" << endl;
                exit(1);
            }
            return (topHalf.top() + bottomHalf.top()) / 2;
        } else {
            cerr << "MedianTracker sizes are not constrained properly" << endl;
            exit(1);
        }
    }
    void addSale(int i) {
        if (!topHalf.size()) {
            topHalf.push(i);
            return;
        }
        if (!bottomHalf.size()) {
            bottomHalf.push(i);
            return;
        }
        if (topHalf.size() == bottomHalf.size()) {
            if (i < bottomHalf.top()) {
                bottomHalf.push(i);
                int tmp = bottomHalf.top();
                bottomHalf.pop();
                topHalf.push(tmp);
            } else {
                topHalf.push(i);
            }
        } else {
            if (i > topHalf.top()) {
                topHalf.push(i);
                int tmp = topHalf.top();
                topHalf.pop();
                bottomHalf.push(tmp);
            } else {
                bottomHalf.push(i);
            }
        }
    }
    int size() {
        return int(topHalf.size()) + int(bottomHalf.size());
    }
    
};

class Market {
private:
    bool verbose = false;
    bool median = false;
    bool clientInfo = false;
    bool timeTraveler = false;
    int lastTimeStamp = 0;
    int numOrdersProcessed = 0;
    vector<StockOrders> allStocks;
    vector<ClientInfo> allClients;
    vector<MedianTracker> allMedians;
    vector<TimeTraveler> allTravelers;
    
    void fillOrder(Order &topOrder, Order &currOrder) {
        int sharesExchanged = min(topOrder.numShares, currOrder.numShares);
        int buyer = currOrder.isBuy ? currOrder.clientNum : topOrder.clientNum;
        int seller = currOrder.isBuy ? topOrder.clientNum : currOrder.clientNum;
        if (verbose) {
            cout << "Client " << buyer << " purchased " << sharesExchanged << " shares of Equity " << currOrder.equityNum << " from Client "
                << seller << " for $" << topOrder.price << "/share" << endl;
        }
        if (median) {
            allMedians[currOrder.equityNum].addSale(topOrder.price);
        }
        if (clientInfo) {
            allClients[buyer].numBought += sharesExchanged;
            allClients[buyer].netVal -= (sharesExchanged * topOrder.price);
            allClients[seller].numSold += sharesExchanged;
            allClients[seller].netVal += (sharesExchanged * topOrder.price);
        }
        currOrder.numShares -= sharesExchanged;
        topOrder.numShares -= sharesExchanged;
    }
public:
    Market(int numStocks, int numClients, bool v, bool m, bool c, bool t) {
        allStocks = vector<StockOrders>(numStocks);
        allClients = vector<ClientInfo>(numClients);
        allMedians = vector<MedianTracker>(numStocks);
        allTravelers = vector<TimeTraveler>(numStocks);
        verbose = v;
        median = m;
        clientInfo = c;
        timeTraveler = t;
    }
    
    int getNumOrdersProcessed() {
        return numOrdersProcessed;
    }
    
    void printMedians() {
        for (int i = 0; i < allMedians.size(); i++) {
            if (allMedians[i].size()) {
                cout << "Median match price of Equity " << i << " at time " << lastTimeStamp << " is " << allMedians[i].getMedian() << endl;
            }
        }
    }
    
    void printClientInfo() {
        cout << "---Client Info---" << endl;
        for (int i = 0; i < allClients.size(); i++) {
            cout << "Client " << i << " bought " << allClients[i].numBought << " and sold " << allClients[i].numSold
                << " for a net transfer of $" << allClients[i].netVal << endl;
        }
    }
    
    void printTravelers() {
        cout << "---Time Travelers---" << endl;
        for (int i = 0; i < allTravelers.size(); i++) {
            cout << "A Time Traveler would have bought shares of Equity " << i << " at time: " << allTravelers[i].buyTime << " and sold these shares at time: " << allTravelers[i].sellTime << endl;
        }
    }
    
    void placeOrder(Order a) {
        if (a.timestamp < 0 || a.clientNum >= allClients.size() || a.clientNum < 0 || a.equityNum >= allStocks.size() || a.equityNum < 0 ||
            a.price < 1 || a.numShares < 1 || a.timestamp < lastTimeStamp) {
            exit(1);
        }
        if (median && lastTimeStamp < a.timestamp) {
            printMedians();
        }
        lastTimeStamp = a.timestamp;
        if (a.isBuy) {
            if (timeTraveler) {
                if (allTravelers[a.equityNum].lowestPriceSeen && a.price - allTravelers[a.equityNum].lowestPriceSeen > allTravelers[a.equityNum].bestProfit) {
                    allTravelers[a.equityNum].bestProfit = a.price - allTravelers[a.equityNum].lowestPriceSeen;
                    allTravelers[a.equityNum].buyTime = allTravelers[a.equityNum].lowestPriceTime;
                    allTravelers[a.equityNum].sellTime = a.timestamp;
                }
            }
            while (a.numShares && !allStocks[a.equityNum].toSell.empty() && allStocks[a.equityNum].toSell.top().price <= a.price) {
                Order topOrder = allStocks[a.equityNum].toSell.top();
                allStocks[a.equityNum].toSell.pop();
                fillOrder(topOrder, a);
                if (topOrder.numShares) {
                    allStocks[a.equityNum].toSell.push(topOrder);
                }
                numOrdersProcessed++;
            }
            if (a.numShares) {
                allStocks[a.equityNum].toBuy.push(a);
            }
        } else {
            if (timeTraveler) {
                if (!allTravelers[a.equityNum].lowestPriceSeen || a.price < allTravelers[a.equityNum].lowestPriceSeen) {
                    allTravelers[a.equityNum].lowestPriceSeen = a.price;
                    allTravelers[a.equityNum].lowestPriceTime = a.timestamp;
                }
            }
            while (a.numShares && !allStocks[a.equityNum].toBuy.empty() && allStocks[a.equityNum].toBuy.top().price >= a.price) {
                Order topOrder = allStocks[a.equityNum].toBuy.top();
                allStocks[a.equityNum].toBuy.pop();
                fillOrder(topOrder, a);
                if (topOrder.numShares) {
                    allStocks[a.equityNum].toBuy.push(topOrder);
                }
                numOrdersProcessed++;
            }
            if (a.numShares) {
                allStocks[a.equityNum].toSell.push(a);
            }
        }
    }
};

#endif /* marketInput_h */
