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
	//double u = ((double)1/mean); 
	return (-(double)1/u)*log(randomNumber());
}


int randomState(double list[],int except,int N_list){
    
    double x = randomNumber();
    double sump =0;
    int number = 0;
    //cout<<"x:"<<x<<endl;
    for(int i=0;i<N_list;i++)
    {
        if(i!=except)
        {
            sump += list[i];
            if(x<=sump)
            {
                number =i;
                break;
            }
        }
        
    }
    return number;

}

int main(int argc, char *argv[]) {

    //cout << "hello" <<endl;
    int N_sim = 100000;

    double et0 = 50;
    double etw = 200;
    double et1 = 20;
    double et2 = 100;
    double ets = 100;

    /*
    double lamda0 = 1/et0;
    double lamdaw = 1/etw;
    double gamma1 = 1/et1;
    double gamma2 = 1/et2;
    double mus = 1/ets;
    */

    double lamda0 = 5;
    double lamdaw = 15;
    double gamma1 = 20;
    double gamma2 = 30;
    double mus = 1/ets;



    double p1 = gamma2/(gamma1+gamma2);
    double p2 = gamma1/(gamma1+gamma2);

    // stationary probability
    double pi0 = lamdaw/(1 + lamda0 + lamdaw);
    double b = lamda0/(gamma1 + gamma2);
    double c = 1+ lamdaw + gamma1 + gamma2;

    double pi1 = (((c-gamma1)/c)*(1+(pi0*(b-1))))-(pi0*b);
    double pi2 = (gamma1/c)*(1+(pi0*(b-1)));
    double sumpi = pi0 + pi1 + pi2;

    cout << "pi0 =" <<pi0 <<endl;
    cout << "pi1 =" <<pi1 <<endl;
    cout << "pi2 =" <<pi2 <<endl;
    cout << "sumpi =" <<sumpi <<endl;

    // calculate Stationary probability
    double pi[] = {pi0,pi1,pi2};
    int count_state[] = {0,0,0};

    // initial Probabilty Matrix
    double state_space[3][3]= {
        {0,p1,p2},
        {lamdaw/(lamdaw+gamma1),0,gamma1/(gamma1+lamdaw)},
        {lamdaw/(lamdaw+gamma2),gamma2/(gamma2+lamdaw),0}
     };   

    for ( int i = 0; i < 3; i++ )
      for ( int j = 0; j < 3; j++ ) {
      
         cout << "P[" << i << "][" << j << "]: ";
         cout << state_space[i][j]<< endl;
      }

    int initstate = 0;
    for(int n=0;n<N_sim;n++)
    {
        initstate = randomState(pi,-1,3);
        count_state[initstate] =count_state[initstate]+1;
        double ts =300;

        do
        {
            //cout << "initstate:"<<initstate<<endl; 
            if(initstate==0) // no WiFi
            {
                double t0 = GenExpo(lamda0);
                ts -=t0;
                if(ts<0)
                {
                    t0+=ts;
                }
                //move state
                double sub_space[3] = {state_space[initstate][0],state_space[initstate][1],state_space[initstate][2]};
               // cout << "sub_space" << endl;
                //cout << sub_space[0] <<" " << sub_space[1] <<" " << sub_space[2] <<endl; 
                initstate = randomState(sub_space,initstate,3);
                count_state[initstate] =count_state[initstate]+1;
            }
            else
            {
                double tw = GenExpo(lamdaw);
                ts -= tw;
                if(ts<0)
                {
                    tw+=ts;
                }
                
                double sub_tw = tw;
                
                do
                {
                    if(initstate==1)
                    {
                        double t1 = GenExpo(gamma1);
                        sub_tw-=t1;
                        if(sub_tw<0)
                        {
                            t1+=sub_tw;
                        }
                        initstate = 2;
                        count_state[initstate] =count_state[initstate]+1;
                    }
                    else
                    {
                        double t2 = GenExpo(gamma2);
                        sub_tw-=t2;
                        if(sub_tw<0)
                        {
                            t2+=sub_tw;
                        }
                        initstate = 2;
                        count_state[initstate] =count_state[initstate]+1;
                    }

                }while(sub_tw>0);
                initstate = 0;
                count_state[initstate] =count_state[initstate]+1;
            }

        }while(ts>0);
    }

    cout << count_state[0] <<endl;
    cout << count_state[1] <<endl;
    cout << count_state[2] <<endl;

    double pop1 = (double)count_state[0]/(double)(count_state[0]+count_state[1]+count_state[2]);
    double pop2 = (double)count_state[1]/(double)(count_state[0]+count_state[1]+count_state[2]);
    double pop3 = (double)count_state[2]/(double)(count_state[0]+count_state[1]+count_state[2]);

    cout <<"P_0 :" << pop1 <<endl;
    cout <<"P_1 :" << pop2 <<endl;
    cout <<"P_2 :" << pop3 <<endl;
    

    /*
     for ( int i = 0; i < 3; i++ )
      for ( int j = 0; j < 3; j++ ) {
      
         cout << "P[" << i << "][" << j << "]: ";
         cout << state_space[i][j]<< endl;
      }
    
    int count_state2[] = {0,0,0};
    int initstate1 = 0;
    for(int i=0;i<N_sim;i++)
    {
        initstate1 = randomState(pi,-1,3);
        count_state2[initstate1]+=1;
    }
   
    cout <<"count_state 0 :" << count_state2[0] <<endl;
    cout <<"count_state 1 :" << count_state2[1] <<endl;
    cout <<"count_state 2 :" << count_state2[2] <<endl;

    double pop11 = (double)count_state2[0]/(double)N_sim;
    double pop21 = (double)count_state2[1]/(double)N_sim;
    double pop31 = (double)count_state2[2]/(double)N_sim;

    cout <<"P_0 :" << pop11 <<endl;
    cout <<"P_1 :" << pop21 <<endl;
    cout <<"P_2 :" << pop31 <<endl;
    */
    return 0;
}