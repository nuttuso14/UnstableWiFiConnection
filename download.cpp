#include <iostream>
#include <cstdio>
#include <time.h> 
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // setprecision
#include <vector>
#include <random>
#include <iterator> 
#include <map> 

using namespace std;

    default_random_engine eng{static_cast<long unsigned int>(time(0))};
    uniform_real_distribution<double> distribution(0.0,1.0);

class RandomNumber{

    private:
    double rate;
    double GenExpo(double u)
    {
        double randomNum = distribution(eng);
        //cout <<"rr: " <<randomNum << endl;
        return (-(double)1/u)*log(randomNum);
    }

    int chooseNumber(double p[],int size)
    {
        double randomNum = distribution(eng);
        double r = randomNum;
        int nums = 0;
        double pi = 0;
        //cout << "r=" << r <<endl;
        for(int i=0;i<size;i++)
        {
            //cout << "P["<<i<<"]="<<p[i] <<endl;
            pi +=p[i];
            //cout << "r=" <<r << ", pi="<<pi <<endl;
            if(r<=pi)
            {
                nums = i;
                break;
            }
        }
        return nums;
    }

    public:
    double mean;
    double ranNumber;
    
    RandomNumber()
    {
        rate = -1;
    }

    RandomNumber(double mean)
    {
        rate = 1/mean;
    }
    double getMean()
    {
        return mean;
    }
    double getRate()
    {
        return rate;
    }
    double getRandomExpo()
    {
        return GenExpo(rate);
    }
    double getNumbers(double p[],int size)
    {
        return chooseNumber(p,size);
    }

};

int main(int argc, char *argv[]) 
{
    int Nsim = 5;
    double ets = 500;
    double et0 = 50;
    double et1 = 70;
    double et2 = 100;
    double etC = 200;
    cout << "download" <<endl;

    vector<double> ts;

    RandomNumber gts(ets);
    RandomNumber gt0(et0);
    RandomNumber gt1(et1);
    RandomNumber gt2(et2);
    RandomNumber gtc(etC);
    RandomNumber ins;

    for(int i=0;i<Nsim;i++)
    {
        //ts[i] = GenExpo(ets);
        ts.push_back(gts.getRandomExpo()); 
    }

    double plist[2] = {et0/(et0+etC),etC/(et0+etC)};
    int wifistatus = ins.getNumbers(plist,2) ;
    double tsi;
    double tnext = -1;
    double tnext2 = -1;
    int useTnext = 0;
    int useTnext2 = 0;
    int chooseWiFirate = 1;
    for(int i=0;i<Nsim;i++)
    {
        tsi = ts[i];
        cout << "====ts : " << tsi <<endl;
        do
        {
        
            if(!wifistatus)
            {
                double t0 = 0;

                if(useTnext)
                {
                    t0 = tnext;
                } 
                else
                {
                    t0 = gt0.getRandomExpo();
                }
                cout << "t0 : " << t0 <<endl;
                tsi -= t0;
                if(tsi<0){
                    tnext = (-1)*tsi;
                    useTnext = 1;
                    cout << "tnext 0 : " << tnext << endl;
                }
                else{
                    wifistatus = 1;
                    useTnext =0;
                }
                
            }
            else{
                double tc =0;
                double t1 = 0;
                double t2 = 0;
                if(useTnext)
                {
                    tc = tnext;
                } 
                else
                {
                    tc = gt0.getRandomExpo();
                    double tt1 = gt1.getRandomExpo();
                    double tt2 = gt2.getRandomExpo();
                    if(tt1<tt2)
                    {
                        chooseWiFirate=1;
                    }
                    else{
                        chooseWiFirate=2;
                    }
                }

                cout << "tc : " << tc <<endl;
                tsi -= tc;
                if(tsi<0)
                {
                    tnext = (-1)*tsi;
                    useTnext = 1;
                    tc += tsi;
                    cout << "tnext c : " << tnext << endl;
                    cout << "tc for loop : " << tc << endl;
                }
                else{
                    wifistatus = 0;
                    useTnext =0;
                }

                do
                {
                    if(chooseWiFirate==1)
                    {
                        if(useTnext2)
                        {
                            t1 = tnext2;
                        }
                        else{
                            t1 = gt1.getRandomExpo(); 
                        }
                        tc-=t1;
                        cout << "t1 : " << t1 <<endl;
                        if(tc<0)
                        {
                            if(useTnext)
                            {
                                tnext2 = -1*tc;
                                useTnext2 = 1;
                                cout << "tnext2 1 : " << tnext2 <<endl;
                            }
                            else
                            {
                                 useTnext2 = 0;
                            }
                            t1+=tc;
                            cout << "new t1 : " << t1 <<endl;
                        }
                        else
                        {
                           chooseWiFirate=2;
                           useTnext2 = 0; 
                        }
                    }
                    else if(chooseWiFirate==2)
                    {
                        if(useTnext2)
                        {
                            t2 = tnext2;
                        }
                        else{
                            t2 = gt2.getRandomExpo(); 
                        }
                        tc-=t2;
                        cout << "t2 : " << t2 <<endl;
                        if(tc<0)
                        {
                            if(useTnext)
                            {
                                tnext2 = -1*tc;
                                useTnext2 = 1;
                                cout << "tnext2 2 : " << tnext2 <<endl;
                            }
                            else
                            {
                                 useTnext2 = 0;
                            }
                            t2+=tc;
                            cout << "new t2 : " << t2 <<endl;
                        }
                        else
                        {
                           chooseWiFirate=1;
                           useTnext2 = 0; 
                        }
                    }
                    

                }while(tc>0);
                
            }

        }while(tsi>0);
    }

    return 0;
}