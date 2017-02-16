#include <iostream>
#include <assert.h>
#include <string.h>
#include <random>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

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
class lootclass{
public:
  double valueTotal;
  double averagevalue;
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
void lootclass::init(){
    double valueTotal=0;
    double averagevalue=0;
}


int main(){

    srand(time(NULL));
    int mu;
    int sigma;
    double epsilon; //exploration chance, 0-1
    double alpha; //memory retaining rate 0-1
    int ans;
    int NArms;
    double averagestart;
    int Npulls;
    bool player;
    int p;


    vector<armm> MrManyArms;
    vector<double> pullsmatrix;
    vector<pullaction> pullhistory;
    vector<double> loot;



    mu =10;
    sigma=1;
    epsilon=0.25; //exploration chance 0-1
    alpha=0.1;  //memory retaining rate 0-1
    NArms= 7; // number of arms of the bandit
    averagestart=100; //base average
    Npulls=1500;

//asks whether player is playing
    cout << "Are you human? 1=yes, 0=no" << endl;
    cin >> player;
    cout << player << endl;

    std::random_device rd; // no clue what this does
    //builds many armed bandit
    for (int i=0; i<NArms;i++){
      armm c;
      c.init();
      c.mu= double( averagestart+30*LYRAND-30*LYRAND);
      c.sigma=averagestart/10+10*LYRAND;
      MrManyArms.push_back(c);
      loot.push_back(100.1); //builds loot vector
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
if (player==1 && Npulls < 8){ //human plays
  for(int i=0; i<Npulls; i++){
    cout << "pick an arm to pull" << endl;
    cin >> p;
    pullaction a;
    a.init();
    a.pullnumber =p;// lever that is being pulled
    a.reward =pullsmatrix[i%p+NArms*i]; //reward of that lever
    pullhistory.push_back(a);//records which lever was pulled and its reward
    loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward;//sets value of arm
  }
}
else{ //robot plays
  for(int i=0; i<Npulls; i++){
    // exploration
    if(LYRAND<epsilon){
      pullaction a;
      int p;
      p=1+floor(NArms*LYRAND);
      a.init();
      a.pullnumber =p;// lever that is being pulled
      a.reward =pullsmatrix[i%p+NArms*i]; //reward of that lever
      pullhistory.push_back(a);
      loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward;//sets value of arm
      cout << "Explored " << p << endl;
    }
    //exploitation
    else{
      pullaction a;

      double max;
      int p;
      max=0;
      for (int j=0; j<loot.size(); j++) //this loop sets p to the index of the max
          {
              if (max<loot[j])
              {
                  max=loot[j];
                  p=j+1;
              }
          }
      a.pullnumber =p;// lever that is being pulled
      a.reward =pullsmatrix[i%p+NArms*i]; //reward of that lever
      pullhistory.push_back(a);
      loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward; //sets value of arm
      cout << "Exploited " << p << endl;
    }
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
    //cout << pullhistory[i].pullnumber << " ";
    //cout << pullhistory[i].reward << endl;

  }
  cout << "Loot vector" << std::endl;
  for (int i=0; i<loot.size(); i++){
    cout << loot[i] <<endl;


    //cout<<"Max value: "<<max<<endl;
  }
}
