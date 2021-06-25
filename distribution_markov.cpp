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
//random_device rd;  //Will be used to obtain a seed for the random number engine
//mt19937 eng(rd());
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
    double ets = 400;
    double et0 = 100;
    double etw = 300;
    double et1 = 60;
    double et2 = 240;
    
    double mus = 1/ets;
    double lamda0 = 1/et0; 
    double gamma1 = 1/et1;
    double gamma2 = 1/et2;
    double lamdaw = 1/etw;

    int debug = 0;

    vector<double> ts; 
    map<int, double> countP; 
    map<int, double> countP2; 
    map<int, double> countPAll; 
    double P0 = et0/(et0+etw); 
    double q1 = et1/(et1+et2);
    double q2 = 1-q1;
    
    
    int wifistatus = 0;
    double use_laststatus = 0;
    double use_laststatus_2 = 0;
    int chooseWiFirate = -1;
    double tnext = -1;
    double tnext2 = -1;
    double countT1 = 0;
    double countT2 = 0;
    double countTall = 0;
    double tsi = 0;
    int wificount = 0;
    int wificount2 = 0;
    int allwificount = 0;
    int useTnext = 0;

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
    double plist[3] = {P0,q1*(1-P0),(q2)*(1-P0)};

    cout << "Plist = "<< plist[0] <<"," << plist[1] << ","<< plist[2] << endl; 
    wifistatus = getWifiStatus(plist,3); 
    cout << "wifistatus="<< wifistatus <<endl;

    for(int i=0;i<Nsim;i++)
    {
        tsi = GenExpo(ets);
        wificount = 0;
        wificount2 = 0;
        allwificount =0;

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
                    tt0 = t0+tsi; 
                }
                else
                { // wifistage changed
                    //double choose_t1 = GenExpo(1/(q1*lamda0));
                   // double choose_t2 = GenExpo(1/(q2*lamda0));
                    //if(choose_t1<choose_t2)
                    //{
                        wifistatus =1;
                    //}
                   // else
                   // {
                   //     wifistatus =2;
                   // }
                
                    use_laststatus = 0; 
                }
                if(debug)
                {
                    cout << " t0:" << tt0 ;
                }
            }
            else
            {
               // cout << "hello Tc" <<endl;
                allwificount++;
                double tc=0;
                double ttc = 0;
                if(use_laststatus)
                {
                    tc = tnext;
                } 
                else
                {
                    tc = GenExpo(etw);
                    double qlist[2] = {q1,q2};
                    chooseWiFirate = (getWifiStatus(qlist,2))+1;  
                }
                ttc = tc;
                tsi -= tc;
                if(tsi<0)
                {
                    tnext = (-1)*tsi;
                    use_laststatus = 1;
                    tc += tsi;
                    ttc = tc;
                    // cout << "tnext c : " << tnext << endl;
                    // cout << "tc for loop : " << tc << endl;
                }
                else
                {
                    wifistatus = 0;
                    use_laststatus =0;
                }
                //cout << "tc :" << ttc <<endl;

                do
                {
                   if(chooseWiFirate==1)
                   {
                      // cout << "hell0 t1" <<endl;
                        double tt1 =0; 
                        double t1 = 0;

                        if(use_laststatus_2)
                        {
                            t1 = tnext2;
                        }
                        else
                        {
                            t1 = GenExpo(et1);
                        }
                        tt1 = t1;
                        tc-=t1;
                        if(tc<0)
                        {
                            if(use_laststatus)
                            {
                                tnext2 = -1*tc;
                                use_laststatus_2 = 1;
                            }
                            else
                            {
                                //cout << "JE T1" <<endl;
                                use_laststatus_2 = 0;
                            }
                             t1+=tc;
                             tt1 = t1;
                        }
                        else
                        {
                            // cout << "chage to t2 " << endl;
                            chooseWiFirate=2;
                            use_laststatus_2 = 0; 
                        }

                         if(debug)
                        {
                            cout << " t1:" << tt1 ;
                        }
                        wificount++;
                        
                   }
                   else if(chooseWiFirate==2)
                   {
                       //cout << "hell0 t2" <<endl;
                       double tt2 = 0;
                       double t2 = 0;
                       if(use_laststatus_2)
                        {
                            t2 = tnext2;
                        }
                        else
                        {
                            t2 = GenExpo(et2);
                        }
                        tt2 = t2;
                        tc-=t2;
                        if(tc<0)
                        {
                            if(use_laststatus)
                            {
                                tnext2 = -1*tc;
                                use_laststatus_2 = 1;
                            }
                            else
                            {
                                //cout << "JE T2" <<endl;
                                use_laststatus_2 = 0;
                            }
                             t2+=tc;
                             tt2 = t2;
                        }
                        else
                        {
                            //cout << "chage to t1 " << endl;
                            chooseWiFirate=1;
                            use_laststatus_2 = 0; 
                        }

                        if(debug)
                        {
                            cout << " t2:" << tt2 ;
                        }
                        wificount2++;
                   }

                }while(tc>0 && use_laststatus_2==0);
                

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

        if(countPAll.find(allwificount)==countPAll.end()) // if there is no P[N=n]
        {
            countPAll.insert(make_pair(allwificount,1));
            countTall++;
        }
        else
        {
            int wcount = countPAll[allwificount];
            countPAll[allwificount] = wcount+1;
            countTall++;
        } 
      
    
        if(debug)
        {
            cout <<endl;
            cout << "State 1 :" << wificount << ":P[N1="<<wificount<<"]="<< countP[wificount] <<endl;
            cout << "State 2 :" << wificount2<< ":P[N2="<<wificount2<<"]="<< countP2[wificount2] <<endl;
            cout << "All State :" << allwificount<< ":P[N="<<allwificount<<"]="<< countPAll[allwificount] <<endl;
        }
        
    }

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

    cout << "========== Matrix ==========" << endl;
    cout << "[ 0 "<< P01 << " " << P02 << " ]" << endl;
    cout << "[ "<< P10 << " 0 " << P12 << " ]" << endl;
    cout << "[ "<< P20 << " "<< P21 << " 0 " << " ]" << endl;


    cout << "=============== Report ==============="<<endl;
    cout << "N_simulation = " <<Nsim << endl;
    cout << "E[t0] = "<< et0 << endl;
    cout << "E[tw] = "<< etw << endl;
    cout << "E[t1] = "<< et1 << endl;
    cout << "E[t2] = "<< et2 << endl;
    cout << "E[ts] = "<< ets << endl;
    cout << "Total State 1 :" << countT1 << endl;
    cout << "Total State 2 :" << countT2 << endl;
    cout << "Total State WiFi :" << countTall << endl;
    cout << "=============== State 1 ==============="<<endl;
    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" # of Visitting | " << setw(15) <<" Simulation |" << setw(15) << " Math Analyisis |" <<endl;
    double sump=0 , summp = 0;
    double pn0 = 0;

    ofstream outfile;
    outfile.open("markov_state1.txt",ios_base::app);
    string content;

    for (map<int, double>::iterator it = countP.begin(); it != countP.end(); ++it) 
    {
        int n = it->first;
        string col1 = "P[" + to_string(n) + "]";
        int n1 = it->second;
        double psim = (double)n1/countT1;
        //cout << col1 << ":" << n1  << ":" << psim <<endl;
        if(n==0)
        {
            pn0 = P0*(1-a01) + P2*(1-a21);
            mp = pn0;
            /*
            cout << "case 0" << endl;
            cout << "pn0 :" << pn0 <<endl;
            cout << "(1-st11)) :" << st11 <<endl;*/
        }
        else
        {
            mp = st11*pow(f1,n-1)*(1-f1);
            //mp = (1-pn0)*pow(f1,n-1)*(1-f1);
        }
        cout << fixed;
        cout << setw(5) <<left << col1 << setprecision(6) << setw(15)<< right << n1 << setw(15)<< right  << psim << setw(15) << setprecision(6)<< right << mp <<endl;
        sump += psim;
        summp +=mp;
        content += to_string(Nsim)+ "," + to_string(ets) + "," + to_string(et0)+ "," + to_string(etw) + "," + to_string(et1) + "," + to_string(et2)  + ","+ to_string(n) \
                  + "," +  to_string(n1) + "," +  to_string(psim) + "," +  to_string(mp) + "\n";
    }
    cout << "sump ="<<sump <<": summp = " << summp <<endl;
    cout << "=============== State 2 ==============="<<endl;
    sump=0;
    summp = 0;
    outfile << content <<"\n"; 
    outfile.close();

    outfile.open("markov_state2.txt",ios_base::app);
    string content2;
    
    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" # of Visitting |" << setw(15) <<" Simulation |" << setw(15) << " Math Analyisis |" <<endl;
    for (map<int, double>::iterator it = countP2.begin(); it != countP2.end(); ++it) 
    {
        int n = it->first;
        string col1 = "P[" + to_string(n) + "]";
        int n2 = it->second;
        double psim = (double)n2/countT2;
        if(n==0)
        {
            mp = P0*(1-a02) + P1*(1-a12);
            pn0 = mp;
        }
        else
        {
            mp = (st21)*pow(f2,n-1)*(1-f2);
            //mp = (1-pn0)*pow(f2,n-1)*(1-f2);
        }
        cout << fixed;
         cout << setw(5) <<left << col1 << setprecision(6) << setw(15)<< right << n2 << setw(15)<< right  << psim << setw(15) << setprecision(6)<< right << mp <<endl;
        sump += psim;
        summp +=mp;
         content2 += to_string(Nsim)+ "," + to_string(ets) + "," + to_string(et0)+ "," + to_string(etw) + "," + to_string(et1) + "," + to_string(et2)  + ","+ to_string(n) \
                  + "," +  to_string(n2) + "," +  to_string(psim) + "," +  to_string(mp) + "\n";

    }
    cout << "sump ="<<sump <<": summp = " << summp <<endl;
    

    outfile << content2 <<"\n"; 
    outfile.close();

    cout << "=============== OVerall ==============="<<endl;

    outfile.open("markov_WiFi.txt",ios_base::app);
    string content3;

    cout <<setw(5) << "P[N=n]| " <<setw(15)<<" # of Visitting |" << setw(15) <<" Simulation |" <<endl;
    for (map<int, double>::iterator it = countPAll.begin(); it != countPAll.end(); ++it) 
    {
        int n = it->first;
        string col1 = "P[" + to_string(n) + "]";
        int n2 = it->second;
        double psim = (double)n2/countTall;
        cout << fixed;
        cout << setw(5) <<left << col1 << setprecision(6) << setw(15)<< right << n2 << setw(15)<< right  << psim << setw(15) <<endl;
        content3 += to_string(Nsim)+ "," + to_string(ets) + "," + to_string(et0)+ "," + to_string(etw) + "," + to_string(et1) + "," + to_string(et2)  + ","+ to_string(n) \
                  + "," +  to_string(n2) + "," +  to_string(psim) + "\n"; 
    }
      
    outfile << content3 <<"\n"; 
    outfile.close();

    cout << "I have hope!"<<endl;
    return 0;
}