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

int main(int argc, char *argv[]) 
{
    
    int Nsim = 50000;
    double ets = 500;
    double et0 = 50;
    double et1 = 50;

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
                 //    cout << "tnext :" << tnext <<endl;
                }
                else // wifistage changed
                {
                    wifistatus =0;
                    use_laststatus = 0;
                }
                wificount++;
            }
        }while(tsi>0);
       // cout << "WiFi count:" << wificount<<endl;
       // cout << "===============================" << (i+1) <<endl;
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
    /*
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) { 
        cout << '\t' << it->first 
             << '\t' << it->second << '\n'; 
    }
    */

   /*
    cout << "=========== Simulation ==============" << endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) { 
        cout << fixed;
        cout << "P(" << it->first <<") ="<< setprecision(8) << (it->second)/Nsim << endl;
    }
    double summ =0;
    cout << "=========== Math ==============" << endl;
    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) {
        int n = it->first;
        double F1 = lamdaw/(lamdaw+mus);
        double F0 = lamda0/(lamda0+mus);
        double c1 = mus/(mus+lamdaw);
        double c0 = lamda0/(lamda0+mus);
        double P1 =(pow(F1*F0,n-1)*(1-F1))*p1;
        double P2 =(pow(F1,n)*pow(F0,n-1)*(1-F0))*p1;
        double P3 =((1-p1)*pow(F0,n))*pow(F1,n-1)*((1-F1));
        double P4 =((1-p1)*(pow(F1*F0,n))*((1-F0)));
        double p = 0;

        if(n==0)
        {
            P4 = (1-p1)*(1-F0);
            p = P4;
        }
        else{
            p = P1+P2+P3+P4; 
        }

        //cout << "P1:"<< P1 <<"," <<"P2:"<<P2 << "," << "P3:" << P3<< "," << "P4:"<<P4 <<endl;
        
        cout << "P(" << n <<") =";
        cout << p << setprecision(8) << endl;
        //cout << "P(" << n <<") =" << p << endl;
        summ +=p;
    }
    cout << "sum :" << summ <<endl;
    */

    ofstream outfile;
    outfile.open("distribution.txt",ios_base::app);
    string content;

    cout << "=========== Report ==============" << endl;
    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" Simulation |" << setw(15) << " Math Analyisis |" <<endl;
    double summ =0;

    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        int n = it->first;
        double F1 = lamdaw/(lamdaw+mus);
        double F0 = lamda0/(lamda0+mus);

       // double P1 =(pow(F1*F0,n-1)*(1-F1))*p1;
       // double P2 =(pow(F1,n)*pow(F0,n-1)*(1-F0))*p1;
       // double P3 =((1-p1)*pow(F0,n))*pow(F1,n-1)*((1-F1));
      //  double P4 =((1-p1)*(pow(F1*F0,n))*((1-F0)));
      double c0 = lamda0/(lamda0+lamdaw);
      double c0c = lamdaw/(lamda0+lamdaw);
      double bigc = (lamda0*lamdaw)/((lamdaw+mus)*(lamda0+mus));
      double c1c = mus/(mus+lamdaw);
      double c10 = mus/(mus+lamda0);
      double P1 = (c0)*(pow(bigc,n-1))*c1c;
      double P2 = (c0)*(pow(F1,n))*(pow(F0,n-1))*c10;
      double P3 = (c0c)*(pow(F0,n))*(pow(F1,n-1))*c1c;
      double P4 = (c0c)*(pow(bigc,n))*c10;
        double p = 0;
       // cout << "P1:" << P1 <<endl;
       // cout << "P2:" << P2 <<endl;
       // cout << "P3:" << P3 <<endl;
       // cout << "P4:" << P4 <<endl;
        if(n==0)
        {
            p = P4;
        }
        else{
            p = P1+P2+P3+P4; 
        }
        cout << fixed;
        string col1 = "P[" + to_string(n) + "]";
        double psim = (it->second)/Nsim;
        cout << setw(5) <<left << col1 << setprecision(6) << setw(15)<< right  << psim << setw(15) << setprecision(6)<< right << p <<endl;
        summ +=p;

        content += to_string(Nsim)+ "," + to_string(ets) + "," + to_string(et0) + "," + to_string(et1) + ","+ to_string(n) \
                  + "," +  to_string(psim) + "," +  to_string(p) + "\n";
    }
    cout << "psum:"<< summ <<endl;

	outfile << content <<"\n"; 
 	outfile.close();
    cout << "Distribution are written in Text file" <<endl;


    // Print Report


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