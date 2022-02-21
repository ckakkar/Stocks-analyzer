#include "investment.h"


investment::investment()
= default;
investment::~investment()
= default;
void investment::setup_investment(ifstream& fin,const string &s)
{
    bool flag=true;
    if(s=="Tesla")
    {
        flag=false;
    }
    cout<<"\n\n\n\nSetting up the investment for "<<s<<": "<<endl<<endl;
    vector<string> v1,v2;
    string num1,num2;
    for(int i=0;i<58;i++)
    {
        getline(fin,num1,',');
        getline(fin,num2,',');
        v1.push_back(num1);
        v2.push_back(num2);
    }

    if(flag)
    {
        string s1,s2;
        for(int i=3;i<v1[0].length();i++)
        {
            s1+=v1[0][i];
        }
        for(int i=3;i<v2[0].length();i++)
        {
            s2+=v2[0][i];
        }
        v1[0]=s1;
        v2[0]=s2;
    }
    for(int i=0;i<58;i++)
    {
        start.push_back(stod(v1[i]));
        close.push_back(stod(v2[i]));
    }
    calcChange();
}
void investment::calcChange()
{
    for(int i=0;i<start.size();i++)
    {
        double num=close[i]-start[i];
        change.push_back(num);
    }
}
void investment::choseDay()
{
    string num;
    cout<<"\n\n\n\n\nBy default we have a data of 58 days fluctuating stock shares. So,"<<endl;
    bab:
    cout<<"\nplease choose within days[14-58] only: ";
    cin>>num;
    if(num.length()>2)
        num.resize(2);
    if((+num[0]<48 || +num[0]>57) && (+num[1]<48 || +num[1]>57))
    {
        cerr<<"\n\nPlease enter digits only"<<endl;
        goto bab;
    }
    int n=stoi(num);
    if(n<14 || n>58)
    {
        cerr<<"\n>>Insufficient data for the entered day<<"<<endl;
        goto bab;
    }
    this->day=n-1;
}