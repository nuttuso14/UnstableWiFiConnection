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

int getWifiStatus(double p1){

    double randomNum = distribution(eng);
    
	double r = 0;
	int wifistatus = 0;
	r = randomNum;
	
	if(r>=(1-p1))
	{
		wifistatus = 1;
	}

	return wifistatus;

}

long double findfactorial(int n)
{
    long double factorial = 1;
    for(int i = 1; i <=n; ++i)
    {
        factorial *= i;
    }

    return factorial;
}

int main(int argc, char *argv[]) 
{
    
    /*int Nsim = 50000;
    int N_count = 0;
    double ets = 500;
    double et0 = 50;
    double et1 = 150;

    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double lamdaw = 1/et1;
    double distrition_time = 2500; */

    if (argc < 6) {
		cerr << "Usage: " << argv[0] << " <SIM_ROUND> " << " <E[ts]> " << "E[T0]" << " <E[Tc]> " << " Distrition_time " << endl;
		return 1;
	}

    int N_count = 0;
    int Nsim = atoi(argv[1]);
    double ets = atof(argv[2]);
    double et0 = atof(argv[3]);
    double et1 = atof(argv[4]);
    double distrition_time = atof(argv[5]); 

    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double lamdaw = 1/et1;
    

    double ts[Nsim];
    map<int, double> countP; 

    double p1= et1/(et0+et1);
    
    for(int i=0;i<Nsim;i++)
    {
        ts[i] = GenExpo(ets);
    }

    // do simulation
    double wifistatus = 0;
    double use_laststatus = 0;
    wifistatus = getWifiStatus(p1);
    double tnext = -1;
    double tw = 0;
    for(int i=0;i<Nsim;i++)
    {
        //cout << "round " << (i+1) <<endl;
        double tsi = ts[i];
        //cout << "ts = <<" <<  tsi <<endl;
        //cout << "===============================" << (i+1) <<endl;
        int wificount = 0;
        tw = 0;
        
        do
        {
            if(!wifistatus)
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
                    wifistatus =1;
                    use_laststatus = 0;
                }
                
            }
            else
            {
                double t1 =0;
                if(use_laststatus)
                {
                    t1 = tnext;
                }
                else
                {
                    t1 = GenExpo(et1);
                }
               // cout << "t1:"<<t1 <<endl;
                tsi -=t1;
                if(tsi<0)
                {
                     tnext = tsi*(-1);
                     use_laststatus = 1;
                     tw = tw + (tsi+t1);
                 //    cout << "tnext :" << tnext <<endl;
                }
                else // wifistage changed
                {
                    wifistatus =0;
                    use_laststatus = 0;
                    tw += t1; 
                }
                wificount++;
            }
        }while(tsi>0);
       // cout << "WiFi count:" << wificount<<endl;
       // cout << "===============================" << (i+1) <<endl;
        
        if(tw<=distrition_time)
        {
            N_count++;
            //cout << "tw:"<<tw<<endl;
            if(countP.find(wificount)==countP.end()) // if there is no P[N=n]
            {
                countP.insert(make_pair(wificount,1));
                //cout << "C["<<wificount<<"]="<<countP[wificount]<<endl;
            }
            else
            {
                int wcount = countP[wificount];
                countP[wificount] = wcount+1;
                //cout << "C["<<wificount<<"]="<<countP[wificount]<<endl;
            }
            
        }

    }


    cout << "================= Report =================" <<endl;
    cout << "=== Simulation ===" <<endl;
    double p_tw = (double)N_count/(double)Nsim;
    cout << "f(tw<="<<distrition_time<<")="<<p_tw<<endl; 
    //cout << "fac(0)=" << findfactorial(0) << endl;
    
    cout << "=== Math Analysis ===" <<endl;
    
    double summ =0;
    map<int, double> recordP; 
    int lastN = 0;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        //wnt =0;
        int n = it->first;
        //cout << n << endl;

        if(n>0)
        {
            double F1 = lamdaw/(lamdaw+mus);
            double F0 = lamda0/(lamda0+mus);
            double c1 = mus/(mus+lamdaw);
            double c0 = lamda0/(lamda0+mus);
            double P1 =(pow(F1*F0,n-1)*(1-F1))*p1;
            double P2 =(pow(F1,n)*pow(F0,n-1)*(1-F0))*p1;
            double P3 =((1-p1)*pow(F0,n))*pow(F1,n-1)*((1-F1));
            double P4 =((1-p1)*(pow(F1*F0,n))*((1-F0)));
            double p = 0;
            p = P1+P2+P3+P4; 
            recordP.insert(make_pair(n,p));
           // cout <<"n:"<< n << ":" << p << endl;
        }
        lastN = n;
    }

    double wnt =0;
    double fwt =0;
    cout << "last:" << lastN << endl;
    cout << findfactorial(lastN)<<endl;
    for(int t=0;t<=distrition_time;t++)
    {
        double ti = t;
        int n=lastN;
        for(int i=1;i<=n;i++)
        {
            double firstterm = pow((lamdaw + mus),i);
            double secondterm = pow(ti,i-1);
            double thirdterm = exp(-1*(lamdaw + mus)*ti);
            long double fac = findfactorial(i-1);
            wnt = (firstterm*secondterm*thirdterm)/fac;      
            
            //cout << "p:" << p <<endl;
            fwt += (wnt*recordP[i]);
        }
    }
    cout << "f(tw<="<<distrition_time<<")="<<fwt<<endl; 
    //cout << "summ:"<<summ <<endl;



    ofstream outfile;
    outfile.open("cdf.txt",ios_base::app);

    string content;
    content +=  to_string(Nsim)+ "," + to_string(ets) + "," + to_string(et0) + "," + to_string(et1) + "," + to_string(distrition_time) + "," + to_string(p_tw) \
            + "," + to_string(fwt); 

	outfile << content <<"\n"; 
 	outfile.close();
    cout << "Distribution are written in Text file" <<endl;


    // Print Report

    return 0;

}