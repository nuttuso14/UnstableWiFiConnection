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

double GenExpo(double u){
	return (-(double)1/u)*log(randomNumber());
}

int getWifiStatus(double p1){



	double r = 0;
	int wifistatus = 0;
	

	r = randomNumber();
	
	if(r<=p1)
	{
		wifistatus = 1;
	}

	return wifistatus;

}


int main(int argc, char *argv[]) {
	
	double P1 = 0.6;
	double t0 =0, t1=0; 
	double u0=2, u1=0.01;
	int NSIM = 10000;
	double sumU0 = 0,sumU1=0;
	for(int i=0;i<NSIM;i++){
		t0 = GenExpo(u0);
		t1 = GenExpo(u1);
		//cout << getWifiStatus(P1) << endl;
		//cout << "t0:"<<t0<<endl;
		//cout << "t1:"<<t1<<endl;
		//cout << "================"<<endl;
		sumU0+=t0;
		sumU1+=t1;
	}
	double avgU0 = sumU0/(double)NSIM;
	double avgU1 = sumU1/(double)NSIM;

	cout << "E[t0]="<<avgU0<<endl;
	cout << "E[t1]="<<avgU1<<endl;


	return 0;
}

