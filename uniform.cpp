// uniform_real_distribution
#include <iostream>
#include <random>
#include <time.h> 

using namespace std;

int main()
{
    const int nrolls=10000;  // number of experiments
    const int nstars=95;     // maximum number of stars to distribute
    const int nintervals=10; // number of intervals

    //default_random_engine generator;
    random_device rd;
    unsigned seed = rd();
    default_random_engine eng{static_cast<long unsigned int>(time(0))};
    //mt19937 gen(rd());

    uniform_real_distribution<double> distribution(0.0,1.0);
    for(int i=0;i<20;i++)
    {
        double r = distribution(eng);
        cout << r << endl; 
    }

  
  return 0;
}