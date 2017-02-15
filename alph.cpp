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
    double mu;
    int sigma;
    void init();
};
class pullaction{
public:
  int pullnumber;
  double reward;
  void init();
};
void armm::init(){
    double mu=0;
    sigma=0;
}
void pullaction::init(){
    double reward=0;
    pullnumber=0;
}


int main(){

    srand(time(NULL));
    int mu;
    int sigma;
    int ans;
    int NArms;
    int averagestart;
    int Npulls;
    bool player;
    int p;


    vector<armm> MrManyArms;
    vector<double> pullsmatrix;
    vector<pullaction> pullhistory;


    mu =10;
    sigma=1;
    NArms= 5; // number of arms of the bandit
    averagestart=100; //base average
    Npulls=9;

//asks whether player is playing
    cout << "Are you human? 1=yes, 0=no" << endl;
    cin >> player;
    cout << player << endl;

    std::random_device rd; // no clue what this does
    //builds many armed bandit
    for (int i=0; i<NArms;i++){
      armm c;
      c.init();
      c.mu= double( averagestart+50*LYRAND-50*LYRAND);
      c.sigma=averagestart/10+30*LYRAND;
      MrManyArms.push_back(c);
    }
    //builds vector of pull rewards, pulls 1st->pulls2nd... pulls 1st again->repeats until Npulls of each arm
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
//game starts here
if (player==1 && Npulls < 8){
  for(int i=0; i<Npulls; i++){
    cout << "pick an arm to pull" << endl;
    cin >> p;
    pullaction a;
    a.init();
    a.pullnumber =p;
    a.reward =pullsmatrix[i%p];
    pullhistory.push_back(a);
  }

}



//outputs the mu and sigma values of the arms
    std::cout << "Arm sigmas and mus" << std::endl;
    for (int i=0; i<MrManyArms.size(); i++){
            //cout << alldecks[i].decklocation << " ";
            cout << MrManyArms[i].mu << " ";
            cout << MrManyArms[i].sigma << endl;
    }
//outputs the pull rewards
    for (int i=0; i<pullsmatrix.size(); i++){
            //cout << pullsmatrix[i] << endl;
    }
//outputs actions and rewards
  std::cout << "Arm pulled and reward" << std::endl;
  for (int i=0; i<Npulls; i++){
    cout << pullhistory[i].pullnumber << " ";
    cout << pullhistory[i].reward << endl;

  }
}
