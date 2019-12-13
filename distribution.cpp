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

int main(int argc, char *argv[]) 
{
    
    int Nsim = 100000;
    double ets = 300;
    double et0 = 10;
    double et1 = 150;

    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double lamdaw = 1/et1;
    int count_p[Nsim] ={0}; 

    double ts[Nsim];
    map<int, double> countP; 

    double p1= et1/(et0+et1);
    
    for(int i=0;i<Nsim;i++)
    {
        ts[i] = GenExpo(ets);
    }

    // do simulation
    double wifistatus = 0;
    wifistatus = getWifiStatus(p1);
    for(int i=0;i<Nsim;i++)
    {
        //cout << "round " << (i+1) <<endl;
        double tsi = ts[i];
        //cout << "ts = <<" <<  tsi <<endl;
        //cout << "===============================" << (i+1) <<endl;
        int wificount = 0;
        
        do
        {
            if(!wifistatus)
            {
                int t0 = GenExpo(et0);
                //cout << "t0:"<<t0 <<endl;
                tsi -=t0;
                wifistatus =1;
            }
            else
            {
                int t1 = GenExpo(et1);
                //cout << "t1:"<<t1 <<endl;
                tsi -=t1;
                wifistatus =0;
                wificount++;
            }
        }while(tsi>0);
        //cout << "WiFi count:" << wificount<<endl;
        //cout << "===============================" << (i+1) <<endl;
        if(countP.find(wificount)==countP.end()) // if there is no P[N=n]
        {
            countP.insert(make_pair(wificount,1));
        }
        else
        {
            int wcount = countP[wificount];
            countP[wificount] = wcount+1;
        }
        
    }
    /*
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) { 
        cout << '\t' << it->first 
             << '\t' << it->second << '\n'; 
    }
    */

    cout << "=========== Simulation ==============" << endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) { 
        cout << "P(" << it->first <<") =" << (it->second)/Nsim << endl;
    }
    cout << "=========== Math ==============" << endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) {
        double n = it->first;
        double F1 = lamdaw/(lamdaw+mus);
        double F0 = mus/(lamda0+mus);
        double c1 = mus/(mus+lamdaw);
        double c0 = lamda0/(lamda0+mus);
        double P1 =(pow(F1*F0,n-1)*c1)*p1;
        double P2 =(pow(F1,n)*pow(F0,n-1)*c0)*p1;
        double P3 =((1-p1)*pow(F0,n))*pow(F1,n-1)*c1;
        double P4 =((1-p1)*(pow(F1*F0,n))*c0);
        double p = P1+P2+P3+P4;
        cout << "P1:"<< P1 <<"," <<"P2:"<<P2 << "," << "P3:" << P3<< "," << "P4:"<<P4 <<endl;
        cout << "P(" << n <<") =" << p << endl;

    }


    /*
    countP.insert(make_pair<int, double>(2, 30)); 
    countP.insert(make_pair<int, double>(10, 30)); 
    //map<int, int>::iterator it = countP.find(1); 
    if(countP.find(10)==countP.end())
    {
        cout << "insert"<<endl;
        countP.insert(make_pair<int, double>(10, 400));
    }
    else
    {
        cout << "Update"<<endl;
        countP[10] = 100;
    }

     for (map<int, int>::iterator it = countP.begin(); it != countP.end(); ++it) { 
        cout << '\t' << it->first 
             << '\t' << it->second << '\n'; 
    } */

    


    return 0;

}