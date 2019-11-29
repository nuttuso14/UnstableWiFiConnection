#include <cstdio>
#include <iostream>
#include "random.h"
using namespace std;

int main(int argc, char *argv[]) {

    Pareto rnd_file_size;

    double p_mean_file_size = 2000;
    double PARETO_SH = 1.8; 

    cout << "Hello pareto" <<endl;

    
    double sum_filesize =0;


    for(int i=0;i<10000;i++){

        double pareto_l = p_mean_file_size*(PARETO_SH-1)/PARETO_SH;
        //cout << p_mean_file_size <<endl;
        //cout << PARETO_SH <<endl;
        rnd_file_size.SetShape(PARETO_SH);
        rnd_file_size.SetLocation(pareto_l);
        double file_size = rnd_file_size++;
        //cout << "file size: "<<file_size <<endl;
        sum_filesize+= (file_size);
    }

    double avg_size = sum_filesize/(double)10000;
    cout << avg_size <<endl;

    return 0;
}