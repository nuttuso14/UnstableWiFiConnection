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
	
	//double P1 = 0.6;
	double t0 =0, t1=0, ts=0, tb=0, tg=0; 
	double mean0= 439, mean1= 1679, session_time =5000;
	double tgood=500, tbad=150;
	int NSIM = 10000;
	double sumsessiontime=0;
	double sumTg = 0,sumTb=0;
	int countP1= 0;
	int wifiStatus = 0;
	double sig0= 0, sig1= 0, sigb = 0, sigG = 0;

	double P1 = mean1/(mean1+mean0);
	double Pg = tgood/(tgood+tbad);

	double sumU0 = 0,sumU1=0;
	double sumOffloadingRatio =0, sumGoodOffloadingRatio=0;

	for(int i=0;i<NSIM;i++)
	{
		sumU0 = 0;
		sumU1=0;
		sumTg = 0;
		sumTb=0;
		//ts = GenExpo(session_time);
		ts = session_time;

		sumsessiontime+=ts;
		wifiStatus = getWifiStatus(P1);
		//cout << "ts: "<<ts << endl;
		do
		{
			
			if(wifiStatus==0)
			{
				t0 = GenExpo(mean0);
				ts = ts - t0;
				
				countP1++;
				if(ts<0)
				{
					t0 = t0 + ts; 
				}
				//cout << "t0: "<< t0 <<endl;
				sumU0+=t0;
				wifiStatus=1;
			}
			else
			{
				t1 = GenExpo(mean1);
				ts = ts - t1;
				if(ts<0)
				{
					t1 = t1 + ts; 
				}
				//cout << "t1: "<< t1 <<endl;
				double sub_t1 = t1;
				int signalstatus =  getWifiStatus(Pg); // 0 is bad 1 is good
				do
				{
					if(signalstatus==0)
					{
						tb = GenExpo(tbad);
						sub_t1 = sub_t1 - tb;
						if(sub_t1<0)
						{
							tb = tb + sub_t1; 
						}
						//cout << "tb: "<< tb <<endl;
						sumTb+=tb;
						signalstatus = 1;
					}
					else
					{
						tg = GenExpo(tgood);
						sub_t1 = sub_t1 - tg;
						if(sub_t1<0)
						{
							tg = tg + sub_t1; 
						}
						//cout << "tg: "<< tg <<endl;
						sumTg+=tg;
						signalstatus = 0;
					}	

				}while(sub_t1>0);

				sumU1+=t1;
				wifiStatus=0;
			}
			
		}while(ts>0);

		double offloadingRatio = sumU1/session_time;
		double goodOffloadingRatio =  sumTg/session_time;
		//cout << "offloadingRatio :" << offloadingRatio <<endl;
		sumOffloadingRatio += offloadingRatio;
		sumGoodOffloadingRatio += goodOffloadingRatio;
		sig0+=sumU0;
		sig1+=sumU1;
		sigb+=sumTb;
		sigG+=sumTg;
		//cout << "Sum t0 : " <<  sumU0 <<endl;
		//cout << "Sum t1 : " <<  sumU1 <<endl;
		//sumTg+=tg;
		//sumTb+=tb;
	}
	double avgU0 = sig0/(double)NSIM;
	double avgU1 = sig1/(double)NSIM;
	//double avgTs = sumsessiontime/(double)NSIM;
	double avgTg = sigG/(double)NSIM;
    double avgTb = sigb/(double)NSIM;
	//double popP1 = (double)countP1/(double)NSIM;
	double avgOffloading_ratio = sumOffloadingRatio/(double)NSIM;
	double avgGoodOffloading_ratio = sumGoodOffloadingRatio/(double)NSIM;

	cout << "======= Report =======" <<endl;
	cout << "Initial P1 with  "<< P1 <<endl;
	cout << "Initial Pg with  "<< Pg <<endl;
	cout << "E[t0]="<<avgU0<<endl;
	cout << "E[t1]="<<avgU1<<endl;
	cout << "E[tg]="<<avgTg<<endl;	
	cout << "E[tb]="<<avgTb<<endl;
	double simP1 = avgU1/(avgU1+avgU0);
	double simPg = avgTg/(avgTg+avgTb);
	cout << "P1 : "<< simP1 <<endl;
	cout << "Pg : "<< simPg <<endl;	
	cout << "======= Model 1 Perfect =======" <<endl;
	cout << "Offloading Ratio = " << avgOffloading_ratio <<endl;
	cout << "======= Model 2 Unstatable=======" <<endl;
	cout << "Offloading Ratio = " << avgGoodOffloading_ratio <<endl;
	
	/*
	cout << "======= Report =======" <<endl;
	cout << "P[initial P1]="<<popP1<<endl;
	cout << "E[t0]="<<avgU0<<endl;
	cout << "E[t1]="<<avgU1<<endl;
	cout << "E[ts]="<<avgTs<<endl;
	cout << "E[tg]="<<avgTg<<endl;	
	cout << "E[tb]="<<avgTb<<endl;	
	cout << "======= Model 1 Perfect =======" <<endl;
	double offloadingRatio = avgU1/avgTs;
	double p1t0 = avgU1/(avgU0+avgU1);
	cout << "Pr[X(t0)=1]="<< p1t0 <<endl;
	cout << "Avareage WiFi Offloading Ratio = " << offloadingRatio <<endl;
	cout << "======= Model 2 Unstatable=======" <<endl;
	double offloadingGoodRatio = avgTg/avgTs;
	double p1tg = avgTg/(avgU0+avgU1);
	cout << "Pr[X(t0)=1]="<< p1tg <<endl;
	cout << "Avareage WiFi Offloading Ratio = " << offloadingGoodRatio <<endl;
	cout << "================================" <<endl;
	*/
	return 0;
}

