//
//  main.cpp
//  Software Design Project
//
//  Created by William Ko on 7/4/17.
//  Copyright © 2017 Will Ko. All rights reserved.
//

#include "Rate.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <ctime>
#include <curl/curl.h>
#include <algorithm>

using namespace std;

const std::string currentTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S %P", &tstruct);
    return buf;
}


// Save data to the local file (to ../Products/Debug/XmlPrseRates folder)
size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// Get data from url, do curls
void getData() {
    std::string filename = "RatesXML.xml";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        FILE *fp = fopen(filename.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, "http://rates.fxcm.com/RatesXML");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

// Extract tag value: <tag>value</tag>
string getTagValue(const string &str,const string &tag) {
    // Before value
    std::string::size_type start = str.find('<'+tag+'>');
    
    if (start != str.npos) {
        // after value
        std::string::size_type end = str.find("</"+tag+'>', start + tag.size() + 2);
        if (end != str.npos) {
            
            std::string::size_type count = end - start-1;
            
            // extract value
            return str.substr(start+tag.size()+2, count-(tag.size()+2)+1);
        }
    }
    return "";
}

int main() {
    
    int delay = 10;
    cout << "\nEnter the time interval (Minutes) \n";
    cin >> delay;
    cin.ignore();
    
    // get data for the first time
    getData();
    
    bool first = true;
    
    string symbol = "GBPUSD";
    float targetRate = 1.30669; // ex

    // drunk, fix later
    do {
        ifstream file("RatesXML.xml");
        string content;
        
        // create empty vector of Rate objects
        vector<Rate> rates;
        
        // parse xml-file
        if (file.is_open()) {
            getline(file, content); //skip <?xml version="1.0" encoding="UTF-8"?>
            getline(file, content); //skip<Rates>
            
            do {
                getline(file, content);
                
                // end of the parsing
                if (content == "</Rates>") {
                    break;
                }
                
                string symbol;
                std::string::size_type end = content.find('>');
                
                if (end != content.npos) {
                    symbol=content.substr(18,end - 19);  //for example, EURUSD
                }
                
                getline(file, content);
                float bid=stof(getTagValue(content, "Bid"));      // <Bid>1.13392</Bid>
                
                getline(file, content);
                float ask=stof(getTagValue(content, "Ask"));      // <Ask>1.13393</Ask>
                
                getline(file, content);
                float high=stof(getTagValue(content, "High"));     // <High>1.13551</High>
                
                getline(file, content);
                float low=stof(getTagValue(content, "Low"));      // <Low>1.13299</Low>
                
                getline(file, content);
                string sdirection=getTagValue(content, "Direction");  // <Direction>1</Direction>
                
                getline(file, content);
                int direction=stoi(sdirection);
                string last=getTagValue(content, "Last");	// <Last>01:10:39</Last>
                
                getline(file, content); //skip</Rates>
                
                //Using the  constructor, create a Rate object
                Rate ob(symbol,bid,ask,high,low,direction,last);
                
                //push the object to the vector of objects
                rates.push_back(ob);
                
                if (first){
                    //display the current object in the first tme
                    ob.display();
                }
                
            } while(true);
            
            if (first) {
                cout << "Welcome to the currency notification system. This program compares your target rate to the bid rate." << endl;
                
                cout << "\nTime interval: " << delay << " (Minutes)" <<endl;
                
                cout << "\nPlease enter the pair <Symbol, Target rate>, ie. <XAGUSD, 15.56>" << endl;
                
                bool vs = false;
                
                while (!vs) {
                    
                    cout << "Symbol: ";
                    cin >> symbol;
                    
                    // Transform to uppercase
                    std::transform(symbol.begin(), symbol.end(),symbol.begin(), ::toupper);
        
                    // for each object in rates
                    for (auto r : rates){
                        if (r.getSymbol() == symbol) {
                            vs = true;
                        }
                    }
                    
                    if (vs == false) {
                        cout << "Invalid input. Symbol must be a string." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    
                }
                
                cout << "Target rate: ";
                while(!(cin >> targetRate)){
                    cout << "Invalid input. Target rate must be a float." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Target rate: ";
                }
                
                first = false;
                
            }
            
        } else {
            // show error message
            cerr << "File not available!";
            cin.get();
            exit(0);
        }
        
        bool found = false;
        bool validSymbol = false;
        
        // for each object in rates
        for (auto r : rates){
            
            if (r.getSymbol() == symbol) {
                validSymbol = true;
            }
            
            // check pair <symbol,target rate>
            if (r.getSymbol() == symbol && targetRate == r.getBid()) {
                
                cout << "Symbol: " << symbol << endl;
                cout << "Target rate: " << targetRate << endl;
                cout << "Bid: " << r.getBid() << endl;
                
                cout <<"\nTarget <"<<symbol<<", "<<targetRate<<"> has been reached!";
                
                // print r object with titles
                r.show();
                found = true;
                break;
            }
        }
        
        if (!validSymbol) {
            cout << "Invalid symbol please try again." << endl;
        }
        
        if(!found){
            cout << "\nTarget <" << symbol << ", " << targetRate << "> NOT been reached!";
            // again after delay min
        }
        
        // lemme smash
        this_thread::sleep_for (std::chrono::minutes(delay));
        
        //for debuging (again after delay sec)
//        this_thread::sleep_for (std::chrono::seconds(delay));
        
        cout << "\nUPDATING DATA " << currentTime()  << endl;;
        getData();

    } while (true);
    
    
    return 0;
}

