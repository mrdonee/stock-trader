//
//  market.cpp
//  mitch
//
//  Created by Mitchell Donahue on 7/29/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//
#include "market.h"
#include <stdio.h>

Market::Market(int numStocks, int numClients, bool v, bool m, bool c, bool t) {
    allStocks = vector<StockOrders>(numStocks);
    allClients = vector<ClientInfo>(numClients);
    allMedians = vector<MedianTracker>(numStocks);
    allTravelers = vector<TimeTraveler>(numStocks);
    verbose = v;
    median = m;
    clientInfo = c;
    timeTraveler = t;
}

void Market::fillOrder(Order &topOrder, Order &currOrder) {
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

void Market::placeOrder(Order a) {
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
