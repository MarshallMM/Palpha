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
void city::init(){
    double latitude=0;
    double longitude=0;
}
double evaluateFitness(vector<int> PolicyF,vector<city> cityVectorF){
  double fitness=0;
  for(int i=0;i<PolicyF.size()-1;i++){
    double distance=0;
    double dx=cityVectorF[PolicyF[i]].longitude-cityVectorF[PolicyF[i+1]].longitude;
    double dy=cityVectorF[PolicyF[i]].latitude-cityVectorF[PolicyF[i+1]].latitude;
    distance=sqrt(dx*dx+dy*dy);
    fitness=fitness+distance;
  }
  return fitness;
}
int main(){
    srand(time(NULL));

    vector<city> cityVector;
    vector<vector<int>> policyVector;
    vector<int>xPolicy;
    int NCities;
    int NPolicies;
    int swap;
    int pick;
    int hold;
    double maxLongitude;
    double maxLatitude;

    NCities=10;
    NPolicies=100;
    maxLatitude= 100;
    maxLongitude= 100;

    //Builds vector of cities and their position
    for(int i=0;i<NCities;i++){
      city c;
      c.init();
      c.latitude=LYRAND*maxLatitude;
      c.longitude=LYRAND*maxLongitude;
      cityVector.push_back(c);
    }

    //Builds initial policy vector with random policies
    for(int i=0;i<NCities;i++){
      xPolicy.push_back(i);
    }
    for(int i=0;i<NPolicies;i++){
      for(int j=0;j<NCities*100;j++){
        //mixes up xPolicy
        pick=rand()%NCities;
        swap=rand()%NCities;
        hold=xPolicy[pick];
        xPolicy[pick]=xPolicy[swap];
        xPolicy[swap]=hold;
      }
      policyVector.push_back(xPolicy);
    }//end of building the initial vector of random policies

    //checks policies dont hit the same city twice
    for(int i=0;i<NPolicies;i++){
      for(int j=0;j<NCities;j++){
        for(int k=0;k<NCities;k++){
          if(j!=k){
            assert(policyVector[i][j]!=policyVector[i][k]);
          }
        }
      }
    }//end of check of policies not hiting same city twice

    for(int i=0;i<NPolicies;i++){
      cout<<evaluateFitness(policyVector[i],cityVector)<<endl;
    }

}
