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
#define LYRAND (double)rand()/RAND_MAX
using namespace std;

int xPos(int state,int hight, int width){// function for getting xpos from state
  return state%width+1;
}
int yPos(int state,int hight, int width){// function for getting ypos from state
  return 1+floor(state/width);
}

int move(int agent_state,int direction, int hight, int width){
  int x;
  int y;

  //gets the x and y positions from the state
  x=agent_state%width+1;
  y=1+floor(agent_state/width);
  //moves position based on direction given
  if(direction==0){//move right
    x=x+1;
    //cout<<"right to x= "<<x<<endl;
  }
  else if(direction==1){//move up
    y=y+1;
    //cout<<"up to y= "<<y<<endl;
  }
  else if(direction==2){//move left
    x=x-1;
    //cout<<"left to x= "<<x<<endl;
  }
  else if(direction==3){//move down
    y=y-1;
    //cout<<"down to y= "<<y<<endl;
  }
//sets position to 1,1 if state is not recognized
  if(agent_state>hight*width || agent_state<0){
    x=1;
    y=1;
  }
//makes sure new position is on gridworld
  if(x<1 || x>width || y<1 ||y>hight){
    x=agent_state%width+1;
    y=1+floor(agent_state/width);
    cout<<"Not moving off gridworld!"<<endl;
  }
  agent_state=(y-1)*width+(x-1);
  return agent_state;
}

int main(){
  srand(time(NULL));
  vector<int> state_vector;
  vector<vector<double>> qTable;
  vector<double> actionQ;
  int hight;
  int width;
  int goal_state;
  int Nstates;
  int agent_start_state;
  int max_iterations;
  bool player;
  int iterations;
  int agent_state;
  int direction;
  int dx;
  int dy;
  double dirmaybe;
  bool ruleOfThumbToggle;
  double epsilon;
  double alpha;
  double actionValue;
  double gamma;
  int agent_old_state;
  double maxQ;


  hight= 53; //hight of the gridworld
  width=80; //width of the gridworld
  epsilon=0.25; //exploration chance 0-1
  alpha=0.1;  //memory retaining rate 0-1
  gamma= 0.9;


  max_iterations=10000; //max number of iterations
  Nstates=hight*width; //number of states
  goal_state=floor(Nstates*LYRAND); //randomly makes a goal state
  agent_start_state=floor(Nstates*LYRAND); //randomly makes a start state
  ruleOfThumbToggle=0; //set to 1 if rule of thumb is to be followed

  iterations=0;
  agent_state=agent_start_state;

  for(int i;i<Nstates;i++){//for Nstates
    //builds state_vector form 0 to nstates

    state_vector.push_back(i);
    //builds qTable
    qTable.push_back({0,0,0,0});
  }

  cout << "Play as human? 1=yes, 0=no" << endl;
  cin >> player;

  while(iterations<max_iterations && agent_state != goal_state){
    iterations++;
    if(player){//human plays
      //imputs an angle in degrees gets a cardinal direction to move from that
      cout << "Pick polar direction in degrees to move: 0=right, 90=up, 180=left, 270=down..." << endl;
      cin >> direction;
      //converts angle to a cardinal direction from 0-3
      direction= floor(direction/90);
      direction= direction%4;

      //outputs old position, cardinal direction traveled
      cout << "old state: " << agent_state <<" xPos: "<< xPos(agent_state,hight,width)<<" yPos: "<< yPos(agent_state,hight,width)<<endl;
      cout <<"cardinal direction (0-3): "<<direction << endl;
      //moves agent
      agent_state=move(agent_state,direction,hight,width);
      //outpusts new position
      cout << "new state: " << agent_state <<" xPos: "<< xPos(agent_state,hight,width)<<" yPos: "<< yPos(agent_state,hight,width)<<endl;
    }
    else if(ruleOfThumbToggle==1){//rule of thumb
      //calculates direction in degrees to the goal from the agents position
      dx=xPos(goal_state,hight,width)-xPos(agent_state,hight,width);
      dy=yPos(goal_state,hight,width)-yPos(agent_state,hight,width);


      //move based on where goal is
      if(dx>0){//move right
        agent_state=move(agent_state,0,hight,width);
        cout<<"right"<<endl;
      }
      else if(dx<0){//move left
        agent_state=move(agent_state,2,hight,width);
        cout<<"left"<<endl;
      }
      else if(dy>0){//move up
        agent_state=move(agent_state,1,hight,width);
        cout<<"up"<<endl;
      }
      else if(dy<0){//move down
        agent_state=move(agent_state,3,hight,width);
        cout<<"down"<<endl;
      }

    }
    else{//q-learner
      // exploration
      if(LYRAND<epsilon){
        direction=floor(LYRAND*4);//picks a random direction
        agent_old_state=agent_state;
        agent_state=move(agent_state,direction,hight,width);

        //values the move
        if(agent_state==goal_state){
          actionValue=100;
        }
        else{
          actionValue=-1;
        }
        actionQ=qTable[agent_state];
        maxQ=0;
        for (int j=0; j<actionQ.size(); j++) //this loop get the maxQ
            {
                if (maxQ<actionQ[j])
                {
                    maxQ=actionQ[j];
                }
            }
        qTable[agent_old_state][direction]=actionValue+gamma*maxQ;
      }
      else{//exploitation

      }
    }
    if(agent_state == goal_state){//reset agent back to start if goal is reached
      agent_state=agent_start_state;
    }
  }//end while
  if(goal_state==agent_state){
    cout<<"goal reached in "<< iterations<< " move(s)"<<endl;
  }

  //outputs start and goal states and positions
  cout << "start state: " << agent_start_state <<" xPos: "<< xPos(agent_start_state,hight,width)<<" yPos: "<< yPos(agent_start_state,hight,width)<<endl;
  cout << "goal state " << goal_state <<" xPos: "<< xPos(goal_state,hight,width)<<" yPos: "<< yPos(goal_state,hight,width)<<endl;
}
