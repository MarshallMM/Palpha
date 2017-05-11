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
#define city_length 30
#define city_width 10
#define city_height 8
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

state_class move(state_class agent,int direction, int width, int length, int height, vector<state_class> buildings){
  int x=agent.xVal;
  int y=agent.yVal;
  int z=agent.zVal;

  if(direction==0){
    agent.xVal=agent.xVal+1;
  }
  else if(direction==1){
    agent.yVal=agent.yVal+1;
  }
  else if(direction==2){
    agent.zVal=agent.zVal+1;
  }
  else if(direction==3){
    agent.xVal=agent.xVal-1;
  }
  else if(direction==4){
    agent.yVal=agent.yVal-1;
  }
  else if(direction==5){
    agent.zVal=agent.zVal-1;
  }
  int state=agent.zVal*length*width+agent.yVal*width+agent.xVal;
//makes sure new position is on gridworld

  if(agent.xVal<0 || agent.xVal>width-1 || agent.yVal<0 ||agent.yVal>length-1 || agent.zVal<0 ||agent.zVal>height-1){
    agent.xVal=x;
    agent.yVal=y;
    agent.zVal=z;
    //cout<<"Not moving off gridworld!"<<endl;
  }
  //makes sure agent is not on a wall
  for(int i=0;i<buildings.size();i++){
    if(buildings[i].xVal==agent.xVal){
      if(buildings[i].yVal==agent.yVal){
        if(buildings[i].zVal==agent.zVal){
          agent.xVal=x;
          agent.yVal=y;
          agent.zVal=z;
          //cout<<"Not moving into wall!"<<endl;
        }
      }
    }
  }
  return agent;
}
int main(){
  srand(time(NULL));
  vector<state_class> buildings;
  vector<vector<double>> qTable;
  vector<double> actionQ;
  vector<int> steps2goal;
  vector<int> walls;
  int rand_integer;
  int Nstates=city_width*city_height*city_length;
  int iterations=0;
  int stepscounter=0;
  int max_iterations=10000000;
  int direction;
  int agent_state;
  int agent_old_state;
  int agent_start_state;
  int goal_state;
  int maxQ;
  int startx=0;
  int starty=LYRAND*city_width;
  int startz=city_height-1;
  double epsilon=0.2;
  double epsilon_decay_rate=1;// multiplied to epsilon after a goal is found
  double alpha=0.1;
  double actionValue;
  double gamma=0.9;
  double maxReward=100;
  //builds agent start
  state_class agent;

  agent.init();
  agent.xVal=startx;
  agent.yVal=starty;
  agent.zVal=startz;
  //builds qTable
  for(int i=0;i<Nstates;i++){
    qTable.push_back({LYRAND,LYRAND,LYRAND,LYRAND,LYRAND,LYRAND});
  }
  //builds city buildings
  for(int i=0; i<city_width;i++){
    for(int j=1; j<city_length;j++){
      rand_integer=floor(LYRAND*city_height/2)+1;
      for(int k=0;k<rand_integer;k++){
        state_class c;
        c.init();
        c.xVal=i;
        c.yVal=j;
        c.zVal=k;
        buildings.push_back(c);
        walls.push_back(c.zVal*city_length*city_width+c.yVal*city_width+c.xVal);
      }
    }
  }
  //builds goal
  state_class goal;
  goal.init();
  goal.xVal=LYRAND*city_width;
  goal.yVal=city_length-1;
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
goal_state=goal.zVal*city_width*city_length+goal.yVal*city_width+goal.xVal;
cout<< "goalz "<<goal.zVal<<endl;
for(int i=0;i<walls.size();i++){
  assert(agent_state!=walls[i]);
  assert(goal_state!=walls[i]);
}
//main loop
while(iterations<max_iterations){
  iterations++;
  stepscounter++;
  agent_state=agent.zVal*(city_width*city_length+0)+agent.yVal*city_width+agent.xVal;
  //cout<<agent_state<<" "<<agent.xVal<<" "<<agent.yVal<<" "<<agent.zVal<<endl;
  if(LYRAND<epsilon){//explore
    direction=floor(LYRAND*6);//picks a random direction
  }
  else{//exploitation
    maxQ=0;
    actionQ=qTable[agent_state];
    for (int j=0; j<actionQ.size(); j++) //this loop get the maxQ index
        {
            if (maxQ<actionQ[j])
            {
                maxQ=actionQ[j];
                direction=j;
                //cout<<"dir "<<j<<endl;
            }
        }
  }//end exploitation
  //cout<<"iterations"<< iterations<< endl;
  agent_old_state=agent_state;
  agent=move(agent,direction,city_width,city_length,city_height,buildings);//moves the agent
  agent_state=agent.zVal*(city_width*city_length)+agent.yVal*city_width+agent.xVal;
  //cout<<agent_state<<" trying for "<< goal_state<<endl;
        //values the move
        if(agent_state==goal_state){
          actionValue=maxReward;
        }
        else{
          actionValue=-1;
        }//end of evaluating move
        actionQ=qTable[agent_state];//gets the vector of actions for the state
        maxQ=0;
        for (int j=0; j<actionQ.size(); j++) //this loop get the maxQ
            {
                if (maxQ<actionQ[j])
                {
                    maxQ=actionQ[j];
                }
            }
        if(agent_state!=agent_start_state && agent_state!=agent_old_state){
          //cout<<"qTable check"<<endl;
          assert(qTable.size()>agent_state);
          qTable[agent_old_state][direction]=qTable[agent_old_state][direction]+alpha*(actionValue+gamma*maxQ-qTable[agent_old_state][direction]);
        }

  if(agent_state == goal_state){//reset agent back to start if goal is reached
    agent.xVal=startx;
    agent.yVal=starty;
    agent.zVal=startz;
    cout<<stepscounter<< endl;
    steps2goal.push_back(stepscounter);//vector of number of steps it took to get to goal
    stepscounter=0;
    agent_state=agent.zVal*(city_width*city_length)+agent.yVal*city_width+agent.xVal;
    epsilon=epsilon*epsilon_decay_rate;
  }
  //cout<<"check4 "<< iterations<< endl;
}//end while


for(int i=0;i<qTable.size();i++){
  for(int j=0;j<6;j++){
    assert(qTable[i][j]<maxReward+1);
  }
}


cout<< "qTable size " <<qTable.size()<<endl;
ofstream out_data("MillerSkempME493steps2goal.txt");
  for (int i=0; i<steps2goal.size(); i++){
    out_data << steps2goal[i] << endl;
  }
  //ofstream out_data("MillerSkempME493BuildingLocations.txt");
    for (int i=0; i<buildings.size(); i++){
      //out_data << buildings[i].xVal<< " "<<buildings[i].yVal<<" "<< buildings[i].zVal << endl;
    }
}//end main
