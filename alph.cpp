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
//distribution function
double rand_normal(double mean, double stddev)
{//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}
bool TestA(double ave1,double ave2, double ave3){
  if (ave1-ave2<0.05*ave3) {
    return 1;
  }
  else{
    return 0;
  }
}
bool TestB(vector<double> value,vector<double> mu){
  double max = 0;
  int B=0;
  int p=0;
  for (int j=0; j<value.size(); j++) //this loop sets p to the index of the max
      {
          if (max<value[j])
          {
              max=value[j];
              B=j;
          }
      }
      max=0;
      for (int j=0; j<mu.size(); j++) //this loop sets p to the index of the max
          {
              if (max<mu[j])
              {
                  max=mu[j];
                  p=j;
              }
          }
  if(B==p){
    return 1;
  }
  else{
    return 0;
  }
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
    int exploretally;

    vector<armm> MrManyArms;
    vector<double> pullsmatrix;
    vector<pullaction> pullhistory;
    vector<double> loot;
    vector<int> pullTally;
    vector<double> pullRewardTotal;
    vector<double> muvector;

    mu =10;
    sigma=1;
    epsilon=0.25; //exploration chance 0-1
    alpha=0.1;  //memory retaining rate 0-1
    NArms= 8; // number of arms of the bandit
    averagestart=100; //base average
    Npulls=1000;
    exploretally=0;

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
      c.sigma=averagestart/10+10*LYRAND;
      MrManyArms.push_back(c);
      loot.push_back(100.1); //builds loot vector
      pullTally.push_back(0); //builds pull tally vector
      pullRewardTotal.push_back(0);// builds reward total vector
      muvector.push_back(MrManyArms[i].mu); //builds mu vector
    }
    //builds vector of pull rewards, pulls 1st->pulls2nd... pulls 1st again->repeats until Npulls of each arm
    for (int i=0; i<Npulls*NArms; i++){
      //r=MrManyArms[i%NArms].mu;
      double x2 = double(rand())/RAND_MAX;
      double x1 = double(rand())/RAND_MAX;
      double x3 =sqrt((-2*log(x1)))*cos(2*3.14159*x2);
      double r = rand_normal(MrManyArms[i%NArms].mu, MrManyArms[i%NArms].sigma);//x3 * MrManyArms[i%NArms].sigma+MrManyArms[i%NArms].mu;

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
    a.reward =pullsmatrix[p-1+NArms*i]; //reward of that lever
    pullhistory.push_back(a);//records which lever was pulled and its reward
    pullTally[p-1]=pullTally[p-1]+1; //adds one to the pull tally
    loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward;//sets value of arm
    pullRewardTotal[p-1]=pullRewardTotal[p-1]+a.reward; // adds reward to reward tally
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
      a.reward =pullsmatrix[p-1+NArms*i]; //reward of that lever
      pullhistory.push_back(a);
      loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward;//sets value of arm
      pullTally[p-1]=pullTally[p-1]+1; //adds one to the pull tally
      pullRewardTotal[p-1]=pullRewardTotal[p-1]+a.reward; // adds reward to reward tally
      //cout << "Explored " << p << endl;
      exploretally=exploretally+1;
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
      a.reward =pullsmatrix[p+NArms*i-1]; //reward of that lever
      pullhistory.push_back(a);
      loot[p-1]= loot[p-1]*(1-alpha)+alpha*a.reward; //sets value of arm
      pullTally[p-1]=pullTally[p-1]+1; //adds one to the pull tally
      pullRewardTotal[p-1]=pullRewardTotal[p-1]+a.reward; // adds reward to reward tally
      //cout << "Exploited " << p << endl;
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
  cout << "Npulls, Average reward, value vector" << exploretally << std::endl;
  for (int i=0; i<loot.size(); i++){
    cout << pullTally[i] << " ";
    cout << pullRewardTotal[i]/pullTally[i]<< " ";
    cout << loot[i] << endl;
  }
//TestA, makes sure rewards average are about equal to the mu of the arm
for (int i=0; i<loot.size(); i++){
  assert(TestA(pullRewardTotal[i]/pullTally[i],MrManyArms[i].mu,averagestart)==1);
}

//TestB, tests the arm with max value equals the arm with largest mu
assert(TestB(loot, muvector));

//outputs 2xnpulls txt with pull and reward
  ofstream out_data("MarshallMillerArmpullandReward.txt");
    for (int i=0; i<pullhistory.size(); i++){

        out_data << pullhistory[i].pullnumber << " ";
        out_data << pullhistory[i].reward << endl;
    }
}
