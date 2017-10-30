//
//  Rate.h
//  Software Design Project
//
//  Created by William Ko on 7/5/17.
//  Copyright © 2017 Will Ko. All rights reserved.
//
#include "Rate.h"

// Constructors
Rate::Rate() {

}
Rate::Rate(string symbol, float bid, float ask, float high, float low, int direction, string last){
	this->symbol=symbol;
	this->bid=bid;
	this->ask=ask;
	this->high=high;
	this->low=low;
	this->direction=direction;
	this->last=last;
}

// Getter/Setter for field symbol
string Rate::getSymbol() const{ 
     return symbol;
}
void Rate::setSymbol(string symbol) { 
     this->symbol=symbol;
}

// Getter/Setter for field bid
float Rate::getBid() const{ 
     return bid;
}
void Rate::setBid(float bid) { 
     this->bid=bid;
}

// Getter/Setter for field ask
float Rate::getAsk() const{ 
     return ask;
}
void Rate::setAsk(float ask) { 
     this->ask=ask;
}

// Getter/Setter for field high
float Rate::getHigh() const{ 
     return high;
}
void Rate::setHigh(float high) { 
     this->high=high;
}

// Getter/Setter for field low
float Rate::getLow() const{ 
     return low;
}
void Rate::setLow(float low) { 
     this->low=low;
}

// Getter/Setter for field direction
int Rate::getDirection() const{ 
     return direction;
}
void Rate::setDirection(int direction) { 
     this->direction=direction;
}

// Getter/Setter for field last
string Rate::getLast() const{ 
     return last;
}
void Rate::setLast(string last) { 
     this->last=last;
}

// Public methods
bool Rate::check(string symbol, float targetRate){    
	return symbol==this->symbol && targetRate>=low && targetRate<=high;
}

void Rate::display(){
    cout <<symbol<<"\t"<<bid<<"\t"<<ask<<"\t"<<high<<"\t"<<low<<"\t"<<direction<<"\t"<<last<< endl;
}

void Rate::show(){
    cout <<"\nsymbol="<<symbol<<" bid="<<bid<<" ask="<<ask<<" high="<<high<<" low="<<low<<" direction="<<direction<<" last="<<last<< endl;
    
}
