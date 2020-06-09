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
    int Nsim = 100000;
    double ets = 1500;
    double et0 = 250;
    double etw = 500;
    double et1 = 50;
    double et2 = 150;

    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double gamma1 = 1/et1;
    double gamma2 = 1/et2;
    double lamdaw = 1/etw;

    int debug = 0;

    vector<double> ts; 
    map<int, double> countP; 
    map<int, double> countP2; 
    double P0 = et0/(et0+et1+et2); 
    double q1 = et1/(et1+et2);
    double q2 = 1-q1;
    double plist[3] = {P0,q1*(1-P0),(q2)*(1-P0)};
   
    //cout << "Plist = "<< plist[0] + plist[1] + plist[2] << endl; 
    int wifistatus = 0;
    double use_laststatus = 0;
    double tnext = -1;
    double countT1 = 0;
    double countT2 = 0;
    double tsi = 0;
    int wificount = 0;
    int wificount2 = 0;
    
    /*  int sumts = 0;
    for(int i=0;i<Nsim;i++)
    {
        tsi = GenExpo(ets);
        sumts+=tsi;
        //cout << tsi <<endl;
    } */
    //cout << sumts/Nsim <<endl;

    wifistatus = getWifiStatus(plist,3); 
  
/*     double tlist[3] = {GenExpo(et0),GenExpo(et1), GenExpo(et2)};

    
    for(int i=1;i<3;i++)
    {
        if(tlist[i]<tlist[i-1])
        {
            wifistatus = i;
        }
    } */
    cout << "wifistatus="<< wifistatus <<endl;
    for(int i=0;i<Nsim;i++)
    {
        tsi = GenExpo(ets);
        wificount = 0;
        wificount2 = 0;
        if(debug)
        {
            cout <<"================" << "Ts = " << tsi <<"================" <<endl;
        }
        do
        {

            if(wifistatus==0)
            {
                double tt0 =0;
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
                tt0 = t0;
                tsi -=t0;
                if(tsi<0)
                {
                    tnext = tsi*(-1);
                    use_laststatus = 1;
                   // cout << "tnext :" << tnext <<endl;
                   tt0 = t0+tsi; 

                }
                else
                { // wifistage changed
                    /*double choose_t1 = GenExpo(1/(q1*lamda0));
                    double choose_t2 = GenExpo(1/(q2*lamda0));
                    if(choose_t1<choose_t2)
                    {
                        wifistatus =1;
                    }
                    else
                    {
                        wifistatus =2;
                    }
                    use_laststatus = 0;*/
                    double p1 = gamma2/(gamma1+gamma2);
                    double p2 = gamma1/(gamma1+gamma2);
                    double qlist[2] = {p1,p2};;
                    
                    use_laststatus = 0;
                    wifistatus = (getWifiStatus(qlist,2))+1;
                }
                if(debug)
                {
                    cout << " t0:" << tt0 ;
                }
            }
            else
            {

               do
               {
                  // cout << "before :" << wifistatus << endl;
                   if(wifistatus==1)
                    {
                       // cout << "that way" << endl;
                        double tt1 =0; 
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
                        //cout << "t1:"<<t1<<endl;
                        tt1 = t1;
                        tsi -=t1;
                        if(tsi<0)
                        {
                          //  cout << "over ts" << endl;
                          //  cout << "ts:"<<ts[i]<<","<< "t1:" << t1 <<endl; 
                            tnext = tsi*(-1);
                            use_laststatus = 1;
                            tt1 = t1+tsi;
                           // cout << "tnext :" << tnext <<endl;
                        }
                        else
                        {
                            //cout << "choose new  status" << endl;
                            /*double choose_t0 = GenExpo(etw);
                            double choose_t2 = GenExpo(et1);
                            if(choose_t0<choose_t2)
                            {
                                wifistatus =0;
                            }
                            else
                            {
                                wifistatus =2;
                            }*/
                            double choose_t10 = lamdaw/(lamdaw+gamma1); 
                            double choose_t12 = gamma1/(lamdaw+gamma1); 
                            double qlist[2] = {choose_t10,choose_t12};;
                    
                            //use_laststatus = 0;
                            wifistatus = (getWifiStatus(qlist,2));
                            if(wifistatus==1)
                            {
                                wifistatus+=1;

                            }
                            use_laststatus = 0;
                        }
                        if(debug)
                        {
                            cout << " t1:" << tt1 ;
                        }
                        wificount++;
                       
                    }
                    else
                    {
                       // cout << "this way" << endl;
                         double t2 = 0;
                         double tt2 =0; 
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
                        //cout << "t2:" <<t2<<endl;
                        tsi -=t2;
                        tt2 =t2; 
                        if(tsi<0)
                        {
                            //cout << "over ts" << endl;
                           // cout << "ts:"<<ts[i]<<","<< "t2:" << t2 <<endl; 
                            tnext = tsi*(-1);
                            use_laststatus = 1;
                             tt2 =t2+tsi; 
                            //cout << "tnext :" << tnext <<endl;
                        }
                        else
                        {
                            //cout << "choose new  status" << endl;
                           /* double choose_t0 = GenExpo(etw);
                            double choose_t1 = GenExpo(et2);
                            if(choose_t0<choose_t1)
                            {
                                wifistatus =0;
                            }
                            else
                            {
                                wifistatus =1;
                            }*/
                            double choose_t20 = lamdaw/(lamdaw+gamma2); 
                            double choose_t21 = gamma2/(lamdaw+gamma2); 
                            double qlist[2] = {choose_t20,choose_t21};;
                    
                            //use_laststatus = 0;
                            wifistatus = (getWifiStatus(qlist,2));
                            use_laststatus = 0;
                        }
                        if(debug)
                        {
                            cout << " t2:" << tt2 ;
                        }
                        wificount2++;
                      
                    }
                    //cout << wifistatus << "," << use_laststatus << endl;
               }while(wifistatus!=0 &&  use_laststatus !=1);

            }
  
        }while(tsi>0);

        if(countP.find(wificount)==countP.end()) // if there is no P[N=n]
        {
            countP.insert(make_pair(wificount,1));
            countT1++;
        }
        else
        {
            int wcount = countP[wificount];
            countP[wificount] = wcount+1;
            countT1++;           
        } 

        if(countP2.find(wificount2)==countP2.end()) // if there is no P[N=n]
        {
            countP2.insert(make_pair(wificount2,1));
            countT2++;
        }
        else
        {
            int wcount = countP2[wificount2];
            countP2[wificount2] = wcount+1;
            countT2++;
        } 
        
        if(debug)
        {
            cout <<endl;
            cout << "State 1 :" << wificount << ":P[N1="<<wificount<<"]="<< countP[wificount] <<endl;
            cout << "State 2 :" << wificount2<< ":P[N2="<<wificount2<<"]="<< countP2[wificount2] <<endl;
        }
    }
    
// set up probability
    //double P0 = et0/(etw + et0);
    //double q1 = et1/(et1+et2);
    //double q2 = et2/(et1+et2);
    double P1 = q1*(1-P0);
    double P2 = q2*(1-P0);
    double P01 = (q1*lamda0)/(lamda0+mus);
    double P02 = (q2*lamda0)/(lamda0+mus);
    double P0T = (mus)/(lamda0+mus);
    double P10 = (lamdaw)/(lamdaw+gamma1+mus);
    double P12 = (gamma1)/(lamdaw+gamma1+mus);
    double P1T = (mus)/(lamdaw+gamma1+mus);
    double P20 = (lamdaw)/(lamdaw+gamma2+mus);
    double P21 = (gamma2)/(lamdaw+gamma2+mus);
    double P2T = (mus)/(lamdaw+gamma2+mus);
    double PTT = 1;

    // set up alpha

    double a21 = ((P01*P20)+P21)/(1-(P02*P20));
    double a01 = P01 + (P02)*a21;
    double a12 = ((P02*P10)+P12)/(1-(P10*P01));
    double a02 = P02 + (P01)*a12;

    // set up f
    double f1 = P10*(a01)+ P12*a21;
    double f2 = P20*(a02)+ P21*a12;
    
    double st10 =  P0 + P2;
    double st11 = (P0*a01)+P1+(P2*a21);
    double st20 =  P0 + P1;
    double st21 =  (P0*a02)+(P1*a12)+P2;
    double mp = 0;
    cout << "=============== Report ==============="<<endl;
    cout << "N_simulation = " <<Nsim << endl;
    cout << "E[t0] = "<< et0 << endl;
    cout << "E[tw] = "<< etw << endl;
    cout << "E[t1] = "<< et1 << endl;
    cout << "E[t] = "<< et2 << endl;
    cout << "E[ts] = "<< ets << endl;
    cout << "Total State 1 :" << countT1 << endl;
    cout << "Total State 2 :" << countT2 << endl;
    cout << "=============== State 1 ==============="<<endl;
    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" # of Visitting " << setw(15) <<" Simulation |" << setw(15) << " Math Analyisis |" <<endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        int n = it->first;
        string col1 = "P[" + to_string(n) + "]";
        int n1 = it->second;
        double psim = (double)n1/countT1;
        //cout << col1 << ":" << n1  << ":" << psim <<endl;
        if(n==0)
        {
            mp = P0*(1-a01) + P2*(1-a21);
        }
        else
        {
            mp = st11*pow(f1,n-1)*(1-f1);
        }
        cout << fixed;
        cout << setw(5) <<left << col1 << setprecision(4) << setw(15)<< right << n1 << setw(15)<< right  << psim << setw(15) << setprecision(4)<< right << mp <<endl;
    }
    cout << "=============== State 2 ==============="<<endl;
    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" # of Visitting " << setw(15) <<" Simulation |" << setw(15) << " Math Analyisis |" <<endl;
    for (map<int, double>::iterator it = countP2.begin(); it != countP2.end(); ++it) 
    {
        int n = it->first;
        string col1 = "P[" + to_string(n) + "]";
        int n2 = it->second;
        double psim = (double)n2/countT2;
        if(n==0)
        {
            mp = P0*(1-a02) + P1*(1-a12);
        }
        else
        {
            mp = st21*pow(f2,n-1)*(1-f2);
        }
        cout << fixed;
         cout << setw(5) <<left << col1 << setprecision(4) << setw(15)<< right << n2 << setw(15)<< right  << psim << setw(15) << setprecision(4)<< right << mp <<endl;
    }
    return 0;
}