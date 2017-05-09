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
#include <math.h>
#include <stdlib.h>
#define LYRAND (double)rand()/RAND_MAX
#define city_length 200
#define city_width 50
#define city_height 20
using namespace std;
class state_class{
public:
    int xVal;
    int yVal;
    int zVal;
    void init();
};
void state_class::init(){
    int xVal=0;
    int yVal=0;
    int zVal=0;
};
int main(){
  srand(time(NULL));
  vector<state_class> buildings;
  int rand_integer;


  //builds agent start
  state_class agent;

  agent.init();
  agent.xVal=0;
  agent.yVal=LYRAND*city_width;
  agent.zVal=city_height;

  //builds city buildings
  for(int i=0; i<city_length;i++){
    for(int j=0; j<city_width;j++){
      rand_integer=floor(LYRAND*city_height);
      for(int k=0;k<rand_integer;k++){
        state_class c;
        c.init();
        c.xVal=i;
        c.yVal=j;
        c.zVal=k;
        buildings.push_back(c);
      }
    }
  }
  //builds goal
  state_class goal;
  goal.init();
  goal.xVal=city_length-1;
  goal.yVal=LYRAND*city_width;
  goal.zVal=0;
  for(int i=0;i<buildings.size();i++){//tests if goal is in a building and raises it too the top
    if(buildings[i].xVal==goal.xVal){
      if(buildings[i].yVal==goal.yVal){
        if(buildings[i].zVal==goal.zVal){
          goal.zVal=goal.zVal+1;
        }
      }
    }
  }
  assert(goal.zVal!=0);
for(int i=0;i<buildings.size();i++){
  if(buildings[i].xVal>198){
    cout<<buildings[i].zVal<<endl;
  }
}
cout<< "goalz"<<goal.zVal<<endl;
}
