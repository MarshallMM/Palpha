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
class city{
public:
    double latitude;
    double longitude;
    void init();
};
void city::init(){//LR_1
    double latitude=0;
    double longitude=0;
}
double evaluateFitness(vector<int> PolicyF,vector<city> cityVectorF){
  double fitness=0;
  for(int i=0;i<PolicyF.size()-1;i++){ //LR_8
    double distance=0;
    double dx=cityVectorF[PolicyF[i]].longitude-cityVectorF[PolicyF[i+1]].longitude;
    double dy=cityVectorF[PolicyF[i]].latitude-cityVectorF[PolicyF[i+1]].latitude;
    distance=sqrt(dx*dx+dy*dy);// LR_7
    fitness=fitness+distance;
  }
  return fitness;//MR_2
}
int main(){
    srand(time(NULL));

    vector<city> cityVector;
    vector<vector<int>> policyVector;
    vector<vector<int>> newPolicyVector;
    vector<int>xPolicy;
    vector<int> tournyOrder;
    vector<double> bestFitnessOfEpisode;
    vector<double> fitnessVector;
    int NCities;
    int NPolicies;
    int swap;
    int pick;
    int hold;
    int bestPolicyN;
    int iterations;
    int max_iterations;

    double maxLongitude;
    double maxLatitude;

    NCities=50;
    NPolicies=100;//HR_2, HR_3, HR_4
    maxLatitude= 100;
    maxLongitude= 100;
    max_iterations= 1000;

    //Builds vector of cities and their position
    for(int i=0;i<NCities;i++){
      city c;
      c.init();
      c.latitude=LYRAND*maxLatitude;
      c.longitude=LYRAND*maxLongitude;
      cityVector.push_back(c);
    }

    //Builds initial policy vector with random policies, MR_1
    //LR_2, LR_3
    for(int i=0;i<NCities;i++){
      xPolicy.push_back(i);
    }
    for(int i=0;i<NPolicies;i++){
      for(int j=0;j<NCities*100;j++){
        //mixes up xPolicy
        pick=rand()%(NCities-1)+1;//ensures the first city never mutates from city 0, LR_5
        swap=rand()%(NCities-1)+1;//ensures the first city never mutates from city 0, LR_5
        hold=xPolicy[pick];
        xPolicy[pick]=xPolicy[swap];
        xPolicy[swap]=hold;
      }
      policyVector.push_back(xPolicy);
    }//end of building the initial vector of random policies

    //builds tournyOrder
    for(int i=0;i<NPolicies*2;i++){
      tournyOrder.push_back(i);
    }
    //end of building tournyOrder

    //checks policies dont hit the same city twice, LR_6
    for(int i=0;i<NPolicies;i++){
      for(int j=0;j<NCities;j++){
        for(int k=0;k<NCities;k++){
          if(j!=k){
            assert(policyVector[i][j]!=policyVector[i][k]);
          }
        }
      }
    }//end of check of policies not hiting same city twice

    //start of main loop

    iterations=0;
    while(iterations<max_iterations){
      //mutate, LR_4
      for(int i=0;i<NPolicies;i++){//mutate
        xPolicy=policyVector[i];
        pick=rand()%(NCities-1)+1; //ensures the first city never mutates from city 0, LR_5
        swap=rand()%(NCities-1)+1; //ensures the first city never mutates from city 0, LR_5
        hold=xPolicy[pick];
        xPolicy[pick]=xPolicy[swap];
        xPolicy[swap]=hold;
        policyVector.push_back(xPolicy);//MR_5, carrying capacity is twice NPolicies
      }
      assert(policyVector.size()==2*NPolicies);
      //end mutate

      //downselect

        //mixes tournyOrder
      for(int i=0;i<tournyOrder.size()*10;i++){
        pick=rand()%tournyOrder.size();
        swap=rand()%tournyOrder.size();
        hold=tournyOrder[pick];
        tournyOrder[pick]=tournyOrder[swap];
        tournyOrder[swap]=hold;
      }
        //binary tournament, MR_4
      for(int i=0; i<NPolicies;i++){
          //if fitness is lower
        if(evaluateFitness(policyVector[tournyOrder[i*2]],cityVector)<evaluateFitness(policyVector[tournyOrder[i*2+1]],cityVector)){
          newPolicyVector.push_back(policyVector[tournyOrder[i*2]]);
        }
        else{
          newPolicyVector.push_back(policyVector[tournyOrder[i*2+1]]);
        }
      }
      policyVector=newPolicyVector;
      newPolicyVector={};
      //end downselect
      //finds and logs best fitness
      double bestFitness;
      double holdDouble;


      bestFitness=1000000;

      for(int i=0;i<NPolicies;i++){
        holdDouble=evaluateFitness(policyVector[i],cityVector);

        if(holdDouble<bestFitness){
          bestFitness=holdDouble;
          bestPolicyN=i;
          assert(1);
        }
      }
      bestFitnessOfEpisode.push_back(bestFitness);
      //end of finding and logging best fitness

      iterations=iterations+1;
    }//end while loop

    for(int i=0;i<NPolicies;i++){
      //fitness of each policy
      //cout<<evaluateFitness(policyVector[i],cityVector)<<endl;

    }
    for(int i=0;i<bestFitnessOfEpisode.size();i++){
      //cout<<bestFitnessOfEpisode[i]<<endl;
    }
    //builds fitnessVector doing this in hindsight for MR_3
    for(int i=0;i<NPolicies;i++){
      fitnessVector.push_back(evaluateFitness(policyVector[i],cityVector));
    }

    //console outputs

    cout<<"First 10 city latitudes and longitudes (out of "<< NCities <<" cities):"<< endl;
    //HR_2
    for(int i=0;i<10 && i<NCities;i++){
      cout<<"City "<<i<<" @ "<<cityVector[i].latitude<<" "<<cityVector[i].longitude<<endl;
    }
    cout<<"Best fitness of first generation: "<<  bestFitnessOfEpisode[1]<< " [length units]"<< endl;
    //HR_1
    cout<<"Best fitness of final generation: "<< bestFitnessOfEpisode.back()<<" [length units]"<<endl;
    cout << "Display best policy? 1=yes, 0=no" << endl;
    bool dispBestPolicy;

    cin >> dispBestPolicy;
    if(dispBestPolicy){//HR_1
      cout<<"One of the best policys was found to be policy "<<bestPolicyN<<endl;
      for(int i=0;i<NCities;i++){
        cout<< "City "<< policyVector[bestPolicyN][i]<<" @ "<<cityVector[policyVector[bestPolicyN][i]].latitude<<" "<<cityVector[policyVector[bestPolicyN][i]].longitude<<endl;
      }
    }
    //Statistical runs logger
    ofstream out_data("MarshallMillersPGammaFitness.txt");
      for (int i=0; i<bestFitnessOfEpisode.size(); i++){

          //out_data << pullhistory[i].pullnumber << " ";
          out_data << bestFitnessOfEpisode[i] << endl;
      }
}
