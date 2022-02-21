#ifndef INVESTMENT_H
#define INVESTMENT_H

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class investment
{
protected:
    vector<double> start,close,change;
    int day;
    void setup_investment(ifstream& fin,const string &s);
    void calcChange();
    void choseDay();
public:
    investment();
    virtual ~investment();
};

#endif
