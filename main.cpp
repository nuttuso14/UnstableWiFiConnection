#include <iostream>
#include <cstdio>
#include <time.h> 
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // setprecision
#include <vector>

using namespace std;

double randomNumber(){

	double r = 0;
	time_t t;
	srand((unsigned) time(&t)+rand());
	r = (double)rand()/(RAND_MAX);
	return r; 
}

double GenExpo(double mean){
	double u = ((double)1/mean); 
	return (-(double)1/u)*log(randomNumber());
}

int getWifiStatus(double p1){

	double r = 0;
	int wifistatus = 0;
	r = randomNumber();
	
	if(r>=(1-p1))
	{
		wifistatus = 1;
	}

	return wifistatus;

}


int main(int argc, char *argv[]) {
	
	double P1 = 0.6;
	double t0 =0, t1=0, ts=0, tb=0, tg=0; 
	double mean0=1000, mean1=2000, session_time =3000;
	double tgood=1500, tbad=500;
	int NSIM = 10000;
	double sumU0 = 0,sumU1=0, sumsessiontime=0;
	double sumTg = 0,sumTb=0;
	int countP1= 0;
	int wifiStatus = 0;

	for(int i=0;i<NSIM;i++)
	{
		t0 = GenExpo(mean0);
		t1 = GenExpo(mean1);
		ts = GenExpo(session_time);
		tg = GenExpo(tgood);
		tb = GenExpo(tbad);

		wifiStatus = getWifiStatus(P1);
		if(wifiStatus==1)
		{
			countP1++;
		}

		sumU0+=t0;
		sumU1+=t1;
		sumsessiontime+=ts;
		sumTg+=tg;
		sumTb+=tb;
	}
	double avgU0 = sumU0/(double)NSIM;
	double avgU1 = sumU1/(double)NSIM;
	double avgTs = sumsessiontime/(double)NSIM;
	double avgTg = sumTg/(double)NSIM;
    double avgTb = sumTb/(double)NSIM;
	double popP1 = (double)countP1/(double)NSIM;

	cout << "======= Report =======" <<endl;
	cout << "P[initial P1]="<<popP1<<endl;
	cout << "E[t0]="<<avgU0<<endl;
	cout << "E[t1]="<<avgU1<<endl;
	cout << "E[ts]="<<avgTs<<endl;
	cout << "E[tg]="<<avgTg<<endl;	
	cout << "E[tb]="<<avgTb<<endl;	
	cout << "======= Model 1 Perfect =======" <<endl;
	double offloadingRatio = avgU1/avgTs;
	cout << "Avareage WiFi Offloading Ratio = " << offloadingRatio <<endl;
	cout << "======= Model 2 Unstatable=======" <<endl;
	double offloadingGoodRatio = avgTg/avgTs;
	cout << "Avareage WiFi Offloading Ratio = " << offloadingGoodRatio <<endl;
	cout << "================================" <<endl;

	return 0;
}

