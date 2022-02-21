#ifndef STOCK_H
#define STOCK_H

#include <vector>
#include "investment.h"

class stocks : public investment
{
private:
    vector<double> RSI,oscilator;
    string name;
    void prediction();
    void trends();
    void getRsi();
    void getOsci();
    int calcPredic();
    double getPos(const int &ind1,const int &ind2);
    double getNeg(const int &ind1,const int &ind2);
    double getlowestclosing(const int &ind1,const int &ind2);
    double gethighestcloing(const int &ind1,const int &ind2);
    void checkPred(const int  &pred);
public:
    stocks();
    stocks(ifstream &fin,const string& s);

    ~stocks();
};

#endif
