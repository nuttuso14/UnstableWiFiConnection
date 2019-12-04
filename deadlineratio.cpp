#include <iostream>
#include <cstdio>
#include <time.h> 
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // setprecision
#include <vector>
#include "random.h"

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

    // input program_name N_sim file_size E[t0] E[t1] E[tb] E[tg] ts b1 b2 bb bg 

    if (argc < 13) {
		cerr << "Usage: " << argv[0] << " <SIM_ROUND> " << " <File_Size> " << "<SHAPE>" << " <E[t0]> " << "<E[t1]>"  << "<E[TauB]>"  \
             << "<E[TauG]>" << "<SESSION_TIME>" << "<B1>" << "<B2>" << "<BB>" << "<BG>" << endl;
		return 1;
	}

    int NSim = atoi(argv[1]);
    double xm = atof(argv[2]);
    double shape = atof(argv[3]);
    double mean0 = atof(argv[4]);
    double mean1 = atof(argv[5]);
    double tbad = atof(argv[6]);
    double tgood = atof(argv[7]);
    double td = atof(argv[8]);
    double b1 = atof(argv[9]);
    double b2 = atof(argv[10]);
    double bb = atof(argv[11]);
    double bg = atof(argv[12]);

    double sumfilesize = 0;
    double P1 = mean1/(mean1+mean0);
	double Pg = tgood/(tgood+tbad);

    Pareto rnd_file_size;

    cout << "P1: " << P1 <<endl;
    cout << "Pg: " << Pg <<endl;

    int count_miss =0;
    int count_miss_1 = 0;
    
    int wifiStatus = 0;
    int wifiStatus1 = 0;
    
    cout << "Lawrence Library" << endl;

    for(int i=0;i<NSim;i++)
    {
        double ts = td, ts1=td;
        double sumtime = 0;

        double pareto_l = xm*(shape-1)/shape;
        //cout << p_mean_file_size <<endl;
        //cout << PARETO_SH <<endl;
        rnd_file_size.SetShape(shape);
        rnd_file_size.SetLocation(pareto_l);
        double file_size = (rnd_file_size++)*8; //MB to Mb 

        double file_size_model_1 = file_size;
        wifiStatus = getWifiStatus(P1);
        wifiStatus1 = getWifiStatus(P1);
        sumfilesize+=file_size;

        //cout << "Start File size : " << file_size<<endl;

            // model 1
            //cout << "Model 1" << endl;
            //cout << "Start File size 1 : " << file_size_model_1<<endl;
            do{
                double tl1 = 0;
                if(!wifiStatus1)
                {
                    tl1 = file_size_model_1/b1;
                    double t0 = GenExpo(mean0);
                    

                    if(tl1>t0){
                        ts1 = ts1 - t0;
                        if(ts1<0)
                        {
                            t0 = t0 + ts1; 
                        }
                        //cout << "t0:"<<t0;
                        double downloaded = t0*b1;
                        file_size_model_1 = file_size_model_1 - (t0*b1);
                        //cout << " downloaded file size:" << downloaded << endl;
                    }
                    else // download finish
                    {    
                        
                        ts1 = ts1 - tl1;
                        if(ts1<0)
                        {
                            tl1 = tl1 + ts1; 
                        }   
                        //cout << "t0 tl:"<<tl1;                
                        if(file_size_model_1>0){
                            double downloaded = tl1*b1;
                            file_size_model_1 = file_size_model_1 - downloaded;
                            //cout << " downloaded file size:" << downloaded << endl;
                        }
                        break;
                    }
                    wifiStatus1 = 1;
                }
                else
                {
                    tl1 = file_size_model_1/(b1+b2);
                    double t1 = GenExpo(mean1);
                    
                    
                    if(tl1>t1){
                        ts1 -=t1;
                        if(ts1<0)
                        {
                            t1 = t1 + ts1; 
                        }
                        //cout << "t1:"<<t1;
                        double downloaded = t1*(b1+b2);
                        file_size_model_1 = file_size_model_1 - (t1*(b1+b2));

                        //cout << " downloaded file size:" << downloaded << endl;

                    }
                    else // download finish
                    {
                        ts1 -=tl1;
                        if(ts1<0)
                        {
                            tl1 = tl1 + ts1; 
                        }
                        //cout << "t1 tl1:"<<tl1;
                        if(file_size_model_1>0)
                        {
                            double downloaded = tl1*(b1+b2);
                            file_size_model_1 = file_size_model_1 - downloaded;
                            //cout << " downloaded file size:" << downloaded << endl;
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
                    
                    

                    if(tl>t0){
                        ts = ts - t0;
                        if(ts<0)
                        {
                            t0 = t0 + ts; 
                        }
                        //cout << "t0:"<<t0;
                        double downloaded = t0*b1;
                        file_size = file_size - (t0*b1);
                       
                        //cout << "downloaded file size:" << downloaded << endl;

                    }
                    else // download finish
                    {
                        
                        ts = ts - tl;
                        if(ts<0)
                        {
                            tl = tl + ts; 
                        }
                        //cout << "t0 tl:"<<tl;
                        if(file_size>0){
                            double downloaded = tl*b1;
                            file_size = file_size - downloaded;
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
       //cout << "File Size:" << file_size << endl;

        if((int)file_size>0)
        {
            count_miss++;
        }
        if((int)file_size_model_1>0)
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
   cout << "==================== Input ====================" << endl;
   cout << "xm :"<<xm << " MB" << endl;
   cout << "shape : " << shape << endl;
   cout <<  "E[t0] = " << mean0 << endl;
   cout <<  "E[t1] = " << mean1 << endl;
   cout <<  "E[tauB] = " << tbad << endl;
   cout <<  "E[tauG] = " << tgood << endl;
   cout <<  "Deadline Time = " << td << endl;
   cout <<  "b1 = " << b1 << endl;
   cout <<  "b2 = " << b2 << endl;
   cout <<  "bb = " << bb << endl; 
   cout <<  "bg = " << bg << endl;    
   cout << "avg. File size: " << avg_fileSize <<endl;
   cout << "==================== Perfect ====================" << endl;
   cout << "Deadline Miss Ratio:"<<deadlineRatio1<<endl;
   cout << "==================== Unstanable ====================" << endl;
   cout << "Deadline Miss Ratio:"<<deadlineRatio<<endl;

    ofstream outfile;
    outfile.open("deadlineRatio.txt",ios_base::app);

	string content;

    content = to_string(NSim) + "," + to_string(xm) + "," + to_string(shape) + "," + to_string(mean0) + "," + to_string(mean1) \
            + "," + to_string(tbad) + "," + to_string(tgood) + "," + to_string(td) + "," + to_string(b1) + "," + to_string(b2) \
            + "," + to_string(bb) + "," + to_string(bg) + "," + to_string(deadlineRatio1) +  "," + to_string(deadlineRatio);

    outfile << content <<"\n"; 
    outfile.close();
    cout << "Results are written in Text file" <<endl;

	return 0;
}

