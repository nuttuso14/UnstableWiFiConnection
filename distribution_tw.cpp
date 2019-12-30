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

double GenExpo(double mean){

    double randomNum = distribution(eng);
    //cout <<"rr: " <<randomNum << endl;
	double u = ((double)1/mean); 
	return (-(double)1/u)*log(randomNum);
}

int getWifiStatus(double p[],int size){

    double randomNum = distribution(eng);
    double r = randomNum;
    int wifistatus = 0;
    double pi = 0;
    //cout << "r=" << r <<endl;
    for(int i=0;i<size;i++)
    {
        //cout << "P["<<i<<"]="<<p[i] <<endl;
        pi +=p[i];
        //cout << "r=" <<r << ", pi="<<pi <<endl;
        if(r<=pi)
        {
            wifistatus = i;
            break;
        }
    }

	return wifistatus;

}

int main(int argc, char *argv[]) 
{
    
    int Nsim = 10000;
    double ets = 500;
    double et0 = 50;
    double et1 = 70;
    double et2 = 100;
    double etw = 200;

    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double gamma1 = 1/et1;
    double gamma2 = 1/et2;


    double ts[Nsim];
    map<int, double> countP; 
    map<int, double> countP2; 

    double plist[3] = {et0/(et0+et1+et2),et1/(et0+et1+et2),et2/(et0+et1+et2)};

    for(int i=0;i<Nsim;i++)
    {
        ts[i] = GenExpo(ets);
    }

    // do simulation
    int wifistatus = 0;
    double use_laststatus = 0;
    wifistatus = getWifiStatus(plist,3);
    //cout << "WiFi status: " << wifistatus <<endl;
    double tnext = -1;

    for(int i=0;i<Nsim;i++)
    {
        double tsi = ts[i];
        do
        {

            if(wifistatus==0)
            {
                double t0 = 0;
                if(use_laststatus)
                {
                    t0 = tnext;
                }
                else
                {
                    t0 = GenExpo(et0);
                }
                //cout << "t0:"<<t0 <<endl;
                tsi -=t0;
                if(tsi<0)
                {
                    tnext = tsi*(-1);
                    use_laststatus = 1;
                    //cout << "tnext :" << tnext <<endl;
                }
                else
                { // wifistage changed
                    double p1 = gamma2/(gamma1+gamma2);
                    double p2 = gamma1/(gamma1+gamma2);

                    double choose_t1 = GenExpo(1/(p1*lamda0));
                    double choose_t2 = GenExpo(1/(p2*lamda0));
                    if(choose_t1<choose_t2)
                    {
                        wifistatus =1;
                    }
                    else
                    {
                        wifistatus =2;
                    }
                    use_laststatus = 0;
                }
            }
            else
            {
                int wificount = 0;
                int wificount2 = 0;
               do
               {
                   //cout << "before :" << wifistatus << endl;
                   if(wifistatus==1)
                    {
                       // cout << "that way" << endl;
                        double t1 = 0;
                        if(use_laststatus)
                        {
                           // cout << "that way last" << endl;
                            t1 = tnext;
                        }
                        else
                        {
                            //cout << "gen t1" << endl;
                            t1 = GenExpo(et1);
                        }

                        tsi -=t1;
                        if(tsi<0)
                        {
                          //  cout << "over ts" << endl;
                          //  cout << "ts:"<<ts[i]<<","<< "t1:" << t1 <<endl; 
                            tnext = tsi*(-1);
                            use_laststatus = 1;
                            //cout << "tnext :" << tnext <<endl;
                        }
                        else
                        {
                            //cout << "choose new  status" << endl;
                            double choose_t0 = GenExpo(etw);
                            double choose_t2 = GenExpo(et1);
                            if(choose_t0<choose_t2)
                            {
                                wifistatus =0;
                            }
                            else
                            {
                                wifistatus =2;
                            }
                            use_laststatus = 0;
                        }
                        wificount++;
                    }
                    else
                    {
                       // cout << "this way" << endl;
                         double t2 = 0;
                        if(use_laststatus)
                        {
                            //cout << "lst this loop" <<endl;
                           
                            t2 = tnext;
                        }
                        else
                        {
                            //cout << "this loop" <<endl;
                            t2 = GenExpo(et2);
                        }

                        tsi -=t2;
                        if(tsi<0)
                        {
                            //cout << "over ts" << endl;
                           // cout << "ts:"<<ts[i]<<","<< "t2:" << t2 <<endl; 
                            tnext = tsi*(-1);
                            use_laststatus = 1;
                            //cout << "tnext :" << tnext <<endl;
                        }
                        else
                        {
                            //cout << "choose new  status" << endl;
                            double choose_t0 = GenExpo(etw);
                            double choose_t1 = GenExpo(et2);
                            if(choose_t0<choose_t1)
                            {
                                wifistatus =0;
                            }
                            else
                            {
                                wifistatus =1;
                            }
                            use_laststatus = 0;
                        }
                        wificount2++;
                    }
                    //cout << wifistatus << "," << use_laststatus << endl;
               }while(wifistatus!=0 &&  use_laststatus !=1);

                if(countP.find(wificount)==countP.end()) // if there is no P[N=n]
                {
                 countP.insert(make_pair(wificount,1));
                }
                else
                {
                    int wcount = countP[wificount];
                    countP[wificount] = wcount+1;
                } 

                if(countP2.find(wificount2)==countP2.end()) // if there is no P[N=n]
                {
                 countP2.insert(make_pair(wificount2,1));
                }
                else
                {
                    int wcount = countP2[wificount2];
                    countP2[wificount2] = wcount+1;
                } 
            }
  
        }while(tsi>0);
    }
    cout << "============ Report ============" <<endl;
    cout << "========= State 1 ==============" <<endl;
    int n_sum = 0;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        int n = it->first;
        n_sum+=(it->second);
        //cout << "P[N="<<n<<"]="<<(it->second)<<endl;
    }
    cout << "n_sum:"<<n_sum<<endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        int n = it->first;
        cout << "P[N="<<n<<"]="<<(it->second)/(double)n_sum<<endl;
    }

    cout << "========= State 2 ==============" <<endl;
    int n_sum2 = 0;
    for (map<int, double>::iterator it = countP2.begin(); it != countP2.end(); ++it) 
    {
        int n = it->first;
        n_sum2+=(it->second);
       // cout << "P[N="<<n<<"]="<<(it->second)<<endl;
    }
    cout << "n_sum2:"<<n_sum2<<endl;
    for (map<int, double>::iterator it = countP2.begin(); it != countP2.end(); ++it) 
    {
        int n = it->first;
        cout << "P[N="<<n<<"]="<<(it->second)/(double)n_sum2<<endl;
    }

    return 0;

}