//
//  median.hpp
//  mitch
//
//  Created by Mitchell Donahue on 7/29/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//

#ifndef median_hpp
#define median_hpp

#include <stdio.h>
#include <queue>
#include <iostream>

using namespace std;

class MedianTracker {
    priority_queue<int, vector<int>, greater<>> topHalf;
    priority_queue<int> bottomHalf;
public:
    int getMedian() const;
    
    void addSale(int i);
    
    int size() const {
        return int(topHalf.size()) + int(bottomHalf.size());
    }
};

#endif /* median_hpp */
