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

    //cout << "Hello1" <<endl;
    int NSim = 10000;
    double xm = 5000;
    double shape = 1.8;
    double sumfilesize = 0; //second
    double b1 = 5; //Mbps
    double b2 = 5; //Mbps
    double bg = 4; //Mbps
    double bb = 1; //Mbps

    double mean0= 225, mean1= 450, td = 600;
	double tgood=150, tbad=75;

    double P1 = mean1/(mean1+mean0);
	double Pg = tgood/(tgood+tbad);

    cout << "P1: " << P1 <<endl;
    cout << "Pg: " << Pg <<endl;

    int count_miss =0;
    int count_miss_1 = 0;
    
    int wifiStatus = 0;
    int wifiStatus1 = 0;
    
    for(int i=0;i<NSim;i++)
    {
        double ts = td, ts1=td;
        double sumtime = 0;
        double file_size = GenPareto(shape,xm);
        double file_size_model_1 = file_size;
        wifiStatus = getWifiStatus(P1);
        wifiStatus1 = getWifiStatus(P1);
        sumfilesize+=file_size;

        //cout << "Start File size : " << file_size<<endl;

            // model 1
           // cout << "Model 1" << endl;
           // cout << "Start File size 1 : " << file_size_model_1<<endl;
            do{
                double tl1 = 0;
                if(!wifiStatus1)
                {
                    tl1 = file_size_model_1/b1;
                    double t0 = GenExpo(mean0);
                    ts1 = ts1 - t0;
                    if(ts1<0)
                    {
                        t0 = t0 + ts1; 
                    }
                    //cout << "t0:"<<t0;

                    if(tl1>t0){
                        double downloaded = t0*b1;
                        file_size_model_1 = file_size_model_1 - (t0*b1);
                       // cout << "downloaded file size:" << downloaded << endl;
                    }
                    else // download finish
                    {                       
                        if(file_size_model_1>0){
                            double downloaded = tl1*b1;
                            file_size_model_1 = file_size_model_1 - downloaded;
                          //  cout << " downloaded file size:" << downloaded << endl;

                        }
                        break;
                    }
                    wifiStatus1 = 1;
                }
                else
                {
                    tl1 = file_size_model_1/(b1+b2);
                    double t1 = GenExpo(mean1);
                    ts1 -=t1;

                    if(ts1<0)
                    {
                        t1 = t1 + ts1; 
                    }
                   // cout << "t1:"<<t1;
                    if(tl1>t1){
                        double downloaded = t1*(b1+b2);
                        file_size_model_1 = file_size_model_1 - (t1*(b1+b2));

                       // cout << " downloaded file size:" << downloaded << endl;

                    }
                    else // download finish
                    {
                        if(file_size_model_1>0)
                        {
                            double downloaded = tl1*(b1+b2);
                            file_size_model_1 = file_size_model_1 - downloaded;
                           // cout << " downloaded file size:" << downloaded << endl;
                        }
                        break;
                    }
                   
                    wifiStatus1 = 0;
                }

            }while(file_size_model_1>0 && ts1>0);


            // model 2
            //cout << "Model 2" << endl;
            //cout << "Start File size: " << file_size<<endl;
            do
            {   
                double tl=0;
                if(!wifiStatus)
                {
                    tl = file_size/b1;
                

                    double t0 = GenExpo(mean0);
                    ts = ts - t0;
                    if(ts<0)
                    {
                        t0 = t0 + ts; 
                    }
                    //cout << "t0:"<<t0;

                    if(tl>t0){
                        double downloaded = t0*b1;
                        file_size = file_size - (t0*b1);
                        sumtime +=t0;
                       // cout << "downloaded file size:" << downloaded << endl;

                    }
                    else // download finish
                    {
                        if(file_size>0){
                            double downloaded = tl*b1;
                            file_size = file_size - downloaded;
                            sumtime +=tl;
                            //cout << " downloaded file size:" << downloaded << endl;


                        }
                        break;
                    }
                    wifiStatus = 1;

                }
                else
                {
                    double t1 = GenExpo(mean1);
                    ts -=t1;

                    if(ts<0)
                    {
                        t1 = t1 + ts; 
                    }

                    double sub_t1 = t1;
                    int signalstatus =  getWifiStatus(Pg);
                    //cout << "t1:"<<t1 <<endl;
                    do{

                        if(!signalstatus)
                        {
                            double tb = GenExpo(tbad);
                            tl = file_size/(b1+bb);
                            
                            if(tl>tb)
                            {
                                sub_t1 -=tb;
                                if(sub_t1<0){
                                    tb+=sub_t1;
                                }
                                //cout << "tb:"<<tb <<endl;
                                double downloaded = tb*(b1+bb);
                                file_size = file_size - (tb*(b1+bb));
                                sumtime +=tb;
                                //cout << " downloaded file size:" << downloaded << endl;
                            }
                            else
                            {
                                sub_t1 -= tl;
                                
                                if(file_size>0)
                                {
                                    if(sub_t1<0)
                                    {
                                        tl+=sub_t1;
                                    }
                                    //cout << "tb tl:"<<tl <<endl;
                                    double downloaded = tl*(b1+bb);
                                    file_size = file_size - downloaded;
                                    sumtime +=tl;
                                    //cout << " downloaded file size:" << downloaded << endl;
                                }
                                break;
                            }
                            signalstatus = 1;
                        }
                        else
                        {
                            double tg = GenExpo(tgood);
                            tl = file_size/(b1+bg);
                        
                            if(tl>tg)
                            {
                                sub_t1 -= tg;
                                if(sub_t1<0){
                                    tg+=sub_t1;
                                }
                                //cout << "tg:"<<tg <<endl;
                                double downloaded = tg*(b1+bg);
                                file_size = file_size - (tg*(b1+bg));
                                sumtime +=tg;
                                //cout << " downloaded file size:" << downloaded << endl;
                            }
                            else{
                                sub_t1 -= tl;
                                if(file_size>0)
                                {
                                    if(sub_t1<0){
                                        tl+=sub_t1;
                                    }
                                    double downloaded = tl*(b1+bg);
                                    //cout << "tg tl:"<<tl <<endl;
                                    file_size = file_size - downloaded;
                                    sumtime +=tl;
                                    //cout << "  downloaded file size:" << downloaded << endl;
                                }
                                break;
                            }
                            signalstatus = 0;
                        }
                    }while(sub_t1>0 && file_size>0);
                    wifiStatus = 0;
                }
                
        }while(file_size>0 && ts>0);

       //cout << "File Size1:" << file_size_model_1 << endl;
       // cout << "File Size:" << file_size << endl;

        if(file_size>0)
        {
            count_miss++;
        }
        if(file_size_model_1>0)
        {
            count_miss_1++;
        }
        
    }

    //cout << "Miss :" << count_miss<<endl;

   double avg_fileSize = sumfilesize/(double)NSim;

   cout << "avg. File size: " << avg_fileSize <<endl;
   double deadlineRatio = (double)count_miss/(double)NSim;
   double deadlineRatio1 = (double)count_miss_1/(double)NSim;
   

   cout << "==================== Report ====================" << endl;
   cout << "==================== Perfect ====================" << endl;
   cout << "Deadline Miss Ratio:"<<deadlineRatio1<<endl;
   cout << "==================== Unstanable ====================" << endl;
   cout << "Deadline Miss Ratio:"<<deadlineRatio<<endl;

	return 0;
}

