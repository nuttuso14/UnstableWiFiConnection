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

double GenPareto(double shape ,double xm){

    double pareto_l = xm*(shape-1)/shape;

	//double u = ((double)1/mean); 
	//return (-(double)1/u)*log(randomNumber());
    return (pareto_l/(pow(1-randomNumber(),1/shape)));
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

    cout << "Hello1" <<endl;
    int NSim = 10000;
    double xm = 10000;
    double shape = 1.8;
    double sumfilesize = 0; //second
    double b1 = 15; //Mbps
    double b2 = 15; //Mbps
    double bg = 15; //Mbps
    double bb = 0.5; //Mbps

    double mean0= 439, mean1= 1679, td =1800;
	double tgood=200, tbad=50;

    double P1 = mean1/(mean1+mean0);
	double Pg = tgood/(tgood+tbad);

    int count_miss =0;
    
    int wifiStatus = 0;
    
    for(int i=0;i<NSim;i++)
    {
        double sumtime = 0;
        double file_size = GenPareto(shape,xm);
        wifiStatus = getWifiStatus(P1);

        //cout << "Start File size : " << file_size<<endl;
            do
            {   
                double tl=0;
                if(!wifiStatus)
                {

                    tl = file_size/b1;
                    double t0 = GenExpo(mean0);
                   // cout << "t0:"<<t0 <<endl;
                    //cout << "tl:"<<tl <<endl;
                    if(tl>t0){
                        file_size = file_size - (t0*b1);
                        sumtime +=t0;
                       // cout << "remain file size:" << file_size << endl;
                       // cout << "current tl:" << sumtime << endl;
                    }
                    else // download finish
                    {
                        if(file_size>0){
                            file_size = file_size - (tl*b1);
                            sumtime +=tl;
                           // cout << "remain file size:" << file_size << endl;
                           // cout << "current tl:" << sumtime << endl;
                        }
                        break;
                    }
                    wifiStatus = 1;

                }
                else
                {
                    double t1 = GenExpo(mean1);
                    double sub_t1 = t1;
                    int signalstatus =  getWifiStatus(Pg);

                    do{

                        if(!signalstatus)
                        {
                            double tb = GenExpo(tbad);
                            tl = file_size/(b1+bb);
                            sub_t1 -=tb;
                            //cout << "tb:"<<tb <<endl;
                            //cout << "tl:"<<tl <<endl;
                            if(tl>tb)
                            {
                                file_size = file_size - (tb*(b1+bb));
                                sumtime +=tb;
                               // cout << "remain file size:" << file_size << endl;
                               // cout << "current tl:" << sumtime << endl;
                            }
                            else
                            {
                                if(file_size>0)
                                {
                                    file_size = file_size - (tl*(b1+bb));
                                    sumtime +=tl;
                                  //  cout << "remain file size:" << file_size << endl;
                                  //  cout << "current tl:" << sumtime << endl;
                                }
                                break;
                            }
                            signalstatus = 1;
                        }
                        else
                        {
                            double tg = GenExpo(tgood);
                            tl = file_size/(b1+bg);
                           // cout << "tg:"<<tg <<endl;
                          //  cout << "tl:"<<tl <<endl;
                            sub_t1 -= tg;
                            if(tl>tg)
                            {
                                file_size = file_size - (tg*(b1+bg));
                                sumtime +=tg;
                               // cout << "remain file size:" << file_size << endl;
                               // cout << "current tl:" << sumtime << endl;
                            }
                            else{
                                if(file_size>0)
                                {
                                    file_size = file_size - (tl*(b1+bg));
                                    sumtime +=tl;
                                   // cout << "remain file size:" << file_size << endl;
                                   // cout << "current tl:" << sumtime << endl;
                                }
                                break;
                            }
                            signalstatus = 0;
                        }
                    }while(sub_t1>0 && file_size>0);
                    wifiStatus = 0;
                }
                
        }while(file_size>0 && td>sumtime);

        //cout << "File Size:" << file_size << endl;

        if(file_size>0)
        {
            count_miss++;
        }
        sumfilesize+=file_size;
    }

    cout << "Miss :" << count_miss<<endl;

    double avg_fileSize = sumfilesize/(double)NSim;

    cout << "avg. File size: " << avg_fileSize <<endl;

	return 0;
}

