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

int main(){
    srand(time(NULL));

    vector<city> cityVector;
    vector<vector<int>> policyVector;
    vector<int>xPolicy;
    int NCities;
    int NPolicies;
    int swap;
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

    //Builds initial policy vector
    for(int i=0;i<NCities;i++){
      xPolicy.push_back(i);
    }
    for(int i=0;i<NPolicies;i++){
      for(int j=0;j<NCities*10;j++){
          //mixes up xPolicy

        
      }
      policyVector.push_back(xPolicy);
    }



}
