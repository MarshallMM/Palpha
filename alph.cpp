#include <iostream>
#include <assert.h>
#include <string.h>
#include <random>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>

#define LYRAND (double)rand()/RAND_MAX

using namespace std;

class armm{
public:
    int mu;
    int sigma;
    void init();
};
void armm::init(){
    double mu=0;
    sigma=0;
}


int main(){

    srand(time(NULL));
    int mu;
    int sigma;
    int ans;
    int NArms;
    int averagestart;
    int Npulls;


    vector<armm> MrManyArms;
    vector<double> pullsmatrix;


    mu =10;
    sigma=1;
    NArms= 5; // number of arms of the bandit
    averagestart=100; //base average
    Npulls=5;



    std::random_device rd; // no clue what this does
    //builds many armed bandit
    for (int i=0; i<NArms;i++){
      armm c;
      c.init();
      c.mu= averagestart+50*LYRAND-50*LYRAND;
      c.sigma=averagestart/10+30*LYRAND;
      MrManyArms.push_back(c);
    }
    //builds vector of pull rewards
    for (int i=0; i<Npulls*NArms; i++){
      //r=MrManyArms[i%NArms].mu;
      double x1 = LYRAND;
      double x2 = LYRAND;
      double x3 =sqrt((-2*log(x1)))*cos(2*3.14159*x2);
      double r = x3 * MrManyArms[i%NArms].sigma+MrManyArms[i%NArms].mu;
      pullsmatrix.push_back(r);

    }


    std::mt19937 e2(rd());

    //ans= std::normal_distribution<< dist(mu, sigma);

    std::cout << "pls send help" << std::endl;
    for (int i=0; i<MrManyArms.size(); i++){
            //cout << alldecks[i].decklocation << " ";
            cout << MrManyArms[i].mu << " ";
            cout << MrManyArms[i].sigma << endl;
    }

    for (int i=0; i<pullsmatrix.size(); i++){

            cout << pullsmatrix[i] << endl;

    }

}
