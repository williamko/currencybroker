//
//  Rate.h
//  Software Design Project
//
//  Created by William Ko on 7/5/17.
//  Copyright © 2017 Will Ko. All rights reserved.
//

#ifndef Rate_H
#define Rate_H

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Rate {
    
    string symbol;  // EURUSD
    float bid;      // <Bid>1.13392</Bid>
    float ask;      // <Ask>1.13393</Ask>
    float high;     // <High>1.13551</High>
    float low;      // <Low>1.13299</Low>
    int direction;  // <Direction>1</Direction>
    string last;	// <Last>01:10:39</Last>
    
public:
    
    Rate();
    Rate(string symbol, float bid, float ask, float high, float low, int direction, string last);
    
    
    // Getter/Setter for field symbol
    string getSymbol()const;
    void setSymbol(string symbol);
    
    
    // Getter/Setter for field bid
    float getBid()const;
    void setBid(float bid);
    
    
    // Getter/Setter for field ask
    float getAsk()const;
    void setAsk(float ask);
    
    
    // Getter & Setter for field high
    float getHigh()const;
    void setHigh(float high);
    
    
    // Getter/Setter for field low
    float getLow()const;
    void setLow(float low);
    
    
    // Getter/Setter for field direction
    int getDirection()const;
    void setDirection(int direction);
    
    
    // Getter/Setter for field last
    string getLast()const;
    void setLast(string last);
    
    
    // Public methods
    bool check(string symbol, float targetRate);
    
    void display();
    void show();
    
};

#endif
