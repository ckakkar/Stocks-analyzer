#include "Stock.h"
#include "gnuplot-cpp/gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}




stocks::stocks()
=default;
stocks::stocks(ifstream &fin,const string& s)
{
    this->name=s;
    setup_investment(fin,s);
    getRsi();
    getOsci();
    this->prediction();
}
stocks::~stocks()
=default;
void stocks::prediction()
{
    bil:
    char num;
    cout<<"\n\n\n";
    cout<<"What do you wanna check for this company?\n"<<endl;
    cout<<"1) Prediction for a specific day"<<endl;
    cout<<"2) General trends"<<endl;
    cout<<"3) Exit sub-section"<<endl<<endl;

    cout<<"Select the number: ";
    cin>>num;
    cin.ignore();
    if(+num!=49 && +num!=50 && +num!=51)
    {
        cerr<<"Please, select from the given options only!"<<endl;
        goto bil;
    }

    switch(+num)
    {
        case 49:
        {
            cout<<"\n\nFor the company: "<<this->name;
            this->choseDay();
            int pred=this->calcPredic();
            this->checkPred(pred);
            break;
        }
        case 50:
        {
            this->trends();
            break;
        }
        case 51:
        {
            cout<<"Bye!!"<<endl;
            return;
        }
    }
    goto bil;
}
void stocks::trends()
{
    char num;
    label:
    cout<<"\n\n\n\n\n";
    cout<<"Which trend you wanna study?\n\n";
    cout<<"1) General stock price Plot"<<endl;
    cout<<"2) RSI Plot"<<endl;
    cout<<"3) Stochastic Oscillator Plot"<<endl;
    cout<<"4) Exit SubSub-section"<<endl<<endl;

    cout<<"Select the number: ";
    cin>>num;
    cin.ignore();
    if(+num!=49 && +num!=50 && +num!=51 && +num!=52)
    {
        cerr<<"Please, select from the given options only!"<<endl;
        goto label;
    }

    switch(+num)
    {
        case 49:
        {

            break;
        }
        case 50:
        {
            Gnuplot g2;
            vector<double> x,y;
            for(int i=14;i<58;i++)
            {
                x.push_back(i);
                y.push_back(RSI[i]);
            }
            g2.set_style("lines");
            g2.set_xlabel("Days[+14]").set_ylabel("RSI");
            string s="RSI plot for "+this->name;
            g2.plot_xy(x,y,s);

            wait_for_key();
            break;
        }
        case 51:
        {
            Gnuplot g3;
            vector<double> x,y;
            for(int i=14;i<58;i++)
            {
                x.push_back(i);
                y.push_back(oscilator[i]);
            }
            g3.set_style("lines");
            g3.set_xlabel("Days[+14]").set_ylabel("OSCI");
            string s="OSCI. plot for "+this->name;
            g3.plot_xy(x,y,s);

            wait_for_key();
            break;
        }
        case 52:
        {
            cout<<"Going Back!!"<<endl;
            return;
        }
    }
    goto label;
}
double stocks::getPos(const int &ind1,const int &ind2)
{
    double avg,sum=0;
    int count=0;
    for(int i=ind1;i<ind2;i++)
    {
        if(change[i]>=0)
        {
            sum+=change[i];
            count++;
        }
    }
    avg=sum/count;
    return avg;
}
double stocks::getNeg(const int &ind1,const int &ind2)
{
    double avg,sum=0;
    int count=0;
    for(int i=ind1;i<ind2;i++)
    {
        if(change[i]<0)
        {
            sum+=change[i];
            count++;
        }
    }
    avg=sum/count;
    return avg;
}
void stocks::getRsi()
{
    for(int i=0;i<14;i++)
    {
        RSI.push_back(-1.0);
    }
    for(int i=14;i<58;i++)
    {
        double avgup=getPos(i-14,i);
        double avgdwn=getNeg(i-14,i);
        double RS=avgup/avgdwn;
        double si=100-(100/(1+abs(RS)));
        RSI.push_back(si);
    }
}
double stocks::getlowestclosing(const int &ind1,const int &ind2)
{
    double min=close[ind1];
    for(int i=ind1;i<ind2;i++)
    {
        if(close[i]<min)
            min=close[i];
    }
    return min;
}
double stocks::gethighestcloing(const int &ind1,const int &ind2)
{
    double max=close[ind1];
    for(int i=ind1;i<ind2;i++)
    {
        if(close[i]>max)
            max=close[i];
    }
    return max;
}
void stocks::getOsci()
{
    for(int i=0;i<14;i++)
    {
        oscilator.push_back(-1.0);
    }
    for(int i=14;i<58;i++)
    {
        double highClose=this->gethighestcloing(i-14,i);
        double lowClose=this->getlowestclosing(i-14,i);
        double closing=close[i];
        double K=((closing-lowClose)/(highClose-lowClose))*100;
        oscilator.push_back(abs(K));
    }
    cout<<endl;
}
int stocks::calcPredic()
{
    double r=RSI[day];
    double o=oscilator[day];
    cout<<"\n\nFor the given day our system tells us that: \n\n";
    cout<<"as here the RSI is "<<r<<" and  Stochastic Oscillator(K) is "<<o<<endl;
    if(r<=30 || o<=30)
    {
        cout<<"Stocks are under priced right now, it would be a smart decision"<<endl;
        cout<<"to buy stocks as they are likely to go up.\n";
        return 1;
    }
    else if(r>=70 || o>=70)
    {
        cout<<"Stocks are over priced right now, it would be a smart decision"<<endl;
         cout<<"to sell stocks as they are likely to go down.\n";
        return -1;
    }
    else if((r>=30 && r<=70) || (o>=30 && o<=70))
    {
        cout<<"Stocks are ambiguos right now, it would be a smart decision"<<endl;
        cout<<"to hold on to stocks as they are likely to stay stable for now.\n";
        return 0;
    }
}
void stocks::checkPred(const int &pred)
{
    int dat=day+1;
    if(dat>58)
    {
        cerr<<"\nNo data available to compare the predictions.\n\n";
        return;
    }
    int poPred;
    double change=start[dat]-close[dat];
    if(change>=0 || pred==0)
    {
        cout<<"\n\nThe real data says that we should have sold the stocks,"<<endl;
        poPred=-1;
    }
    else
    {
        cout<<"\n\nThe real data says that we should have sold the stocks,"<<endl;
        poPred=1;
    }
    if(poPred==pred)
        cout<<"And our prediction was correct."<<endl;
    else
        cout<<"And our prediction was incorrect."<<endl;
}
void menu()
{
    char num;
    cout<<">>>>>>>>>>Welcome to the Stock Market Analysis tool<<<<<<<<<<<";
    label:
    cout<<"\n\n\n\n";
    cout<<"What company's stocks are you trying to inspect?\n"<<endl;
    cout<<"1) AMD"<<endl;
    cout<<"2) Tesla"<<endl;
    cout<<"3) Nvidia"<<endl;
    cout<<"4) Exit App"<<endl<<endl;

    cout<<"Select the number: ";
    cin>>num;
    cin.ignore();
    if(+num!=49 && +num!=50 && +num!=51 && +num!=52)
    {
        cerr<<"Please, select from the given options only!"<<endl;
        goto label;
    }

    switch(+num)
    {
        case 49:
        {
            ifstream finA("/Users/cyruskakkar/Downloads/lab4Files/amd.csv");
            if(finA.fail())
            {
                cerr<<"\nError while opening the AMD stocks file."<<endl;
                goto label;
            }
            stocks *skA=new stocks(finA,"AMD");
            delete skA;
            break;
        }
        case 50:
        {
            ifstream finT("/Users/cyruskakkar/Downloads/lab4Files/tesla.csv");
            if(finT.fail())
            {
                cerr<<"\nError while opening the Tesla stocks file."<<endl;
                goto label;
            }
            stocks *skT=new stocks(finT,"Tesla");
            delete skT;
            break;
        }
        case 51:
        {
            ifstream finN("/Users/cyruskakkar/Downloads/lab4Files/nvda.csv");
            if(finN.fail())
            {
                cerr<<"\nError while opening the Nvidia stocks file."<<endl;
                goto label;
            }
            stocks *skN=new stocks(finN,"Nvidia");
            delete skN;
            break;
        }
        case 52:
        {
            cout<<"Bye bro!"<<endl;
            return;
        }
    }
    goto label;
}