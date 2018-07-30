//
//  marketInput.h
//  mitch
//
//  Created by Mitchell Donahue on 7/27/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//

#ifndef marketInput_h
#define marketInput_h

#include "structs.h"
#include "median.h"
#include <iostream>
#include <vector>
using namespace std;

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
    
    void fillOrder(Order &topOrder, Order &currOrder);
public:
    Market(int numStocks, int numClients, bool v, bool m, bool c, bool t);
    
    int getNumOrdersProcessed() const {
        return numOrdersProcessed;
    }
    
    void printMedians() const {
        for (int i = 0; i < allMedians.size(); i++) {
            if (allMedians[i].size()) {
                cout << "Median match price of Equity " << i << " at time " << lastTimeStamp << " is " << allMedians[i].getMedian() << endl;
            }
        }
    }
    
    void printClientInfo() const {
        cout << "---Client Info---" << endl;
        for (int i = 0; i < allClients.size(); i++) {
            cout << "Client " << i << " bought " << allClients[i].numBought << " and sold " << allClients[i].numSold
                << " for a net transfer of $" << allClients[i].netVal << endl;
        }
    }
    
    void printTravelers() const {
        cout << "---Time Travelers---" << endl;
        for (int i = 0; i < allTravelers.size(); i++) {
            cout << "A Time Traveler would have bought shares of Equity " << i << " at time: " << allTravelers[i].buyTime << " and sold these shares at time: " << allTravelers[i].sellTime << endl;
        }
    }
    
    void placeOrder(Order a);
};

#endif /* marketInput_h */
