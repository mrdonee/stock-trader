//
//  median.cpp
//  mitch
//
//  Created by Mitchell Donahue on 7/29/18.
//  Copyright © 2018 Mitchell Donahue. All rights reserved.
//

#include "median.h"

int MedianTracker::getMedian() const {
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

void MedianTracker::addSale(int i) {
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
