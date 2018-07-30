#include "market.h"
using namespace std;

vector<Order> getOrders() {
    vector<Order> orders;
    orders.push_back(Order(0, 1, true, 0, 81, 11));
    orders.push_back(Order(1, 1, true, 1, 92, 15));
    orders.push_back(Order(1, 0, true, 1, 15, 26));
    orders.push_back(Order(2, 0, true, 0, 7, 2));
    orders.push_back(Order(2, 2, false, 0, 71, 45));
    orders.push_back(Order(3, 1, true, 1, 83, 20));
    orders.push_back(Order(3, 2, true, 0, 75, 24));
    orders.push_back(Order(4, 1, true, 1, 84, 17));
    orders.push_back(Order(4, 2, false, 1, 5, 50));
    orders.push_back(Order(4, 0, true, 1, 13, 17));
    orders.push_back(Order(4, 0, false, 1, 58, 45));
    orders.push_back(Order(4, 1, true, 0, 17, 49));
    return orders;
    
}

int main() {
    int numEquities = 2;
    int numClients = 3;
    bool verbose = true;
    bool median = true;
    bool clientInfo = true;
    bool timeTraveler = true;
    cout << "Processing orders..." << endl;
    Market stockMarket = Market(numEquities, numClients, verbose, median, clientInfo, timeTraveler);
    vector<Order> orders = getOrders();
    for(int i = 0; i < orders.size(); i++) {
        stockMarket.placeOrder(orders[i]);
    }
    if (median) {
        stockMarket.printMedians();
    }
    cout << "---End of Day---" << endl;
    cout << "Orders Processed: " << stockMarket.getNumOrdersProcessed() << endl;
    if (clientInfo) {
        stockMarket.printClientInfo();
    }
    if (timeTraveler) {
        stockMarket.printTravelers();
    }
    return 0;
}
                   
                   
                   
                   
                   
                   
                   
                   
                   
                   
                   
                   
