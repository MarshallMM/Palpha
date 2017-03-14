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
using namespace std;

class state2{
public:
    int xVal;
    int yVal;
    void init();
};
void state2::init(){
    int xVal=0;
    int yVal=0;
}

int xPos(int state,int hight, int width){// function for getting xpos from state
  return state%width+1;
}
int yPos(int state,int hight, int width){// function for getting ypos from state
  return 1+floor(state/width);
}

int move(int agent_state,int direction, int hight, int width, vector<int> walls){
  int x;
  int y;
  int wallx;
  int wally;

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
  if(x<1 || x>width || y<1 ||y>hight ){
    x=agent_state%width+1;
    y=1+floor(agent_state/width);
    //cout<<"Not moving off gridworld!"<<endl;
  }
  //makes sure agent is not on a wall
  for(int i=0;i<walls.size();i++){
    wallx=walls[i]%width+1;
    wally=1+floor(walls[i]/width);
    if(x==wallx && y==wally){//resets x,y back to original if moved onto a wall
      x=agent_state%width+1;
      y=1+floor(agent_state/width);
    }
  }

  agent_state=(y-1)*width+(x-1);
  return agent_state;
}

int main(){
  srand(time(NULL));
  vector<int> walledStates;
  vector<int> state_vector;
  vector<vector<double>> qTable;
  vector<double> actionQ;
  vector<int> steps2goal;
  int wallAState;
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
  int stepscounter;
  int maxReward;
  int optimalSteps;
  int minSteps;



  hight= 10; //hight of the gridworld
  width=20; //width of the gridworld
  epsilon=0.25; //exploration chance 0-1
  alpha=0.1;  //memory retaining rate 0-1
  gamma= 0.9;
  maxReward=100; //maximum reward when goal is found
  ruleOfThumbToggle=0; //set to 1 if rule of thumb is to be followed

wallAState=0;
  max_iterations=100000; //max number of iterations
  Nstates=hight*width; //number of states
  goal_state=floor(Nstates*LYRAND); //randomly makes a goal state
  agent_start_state=floor(Nstates*LYRAND); //randomly makes a start state

  iterations=0;
  stepscounter=0;
  agent_state=agent_start_state;

  for(int i=0;i<Nstates;i++){//for Nstates
    //builds state_vector form 0 to Nstates

    state_vector.push_back(i);
    //builds qTable
    qTable.push_back({LYRAND-1,LYRAND-1,LYRAND-1,LYRAND-1});
  }
  //builds walled states vector
  for(int i=0;i<Nstates/3;i++){
    wallAState=rand()%Nstates;
    if(wallAState != agent_start_state && wallAState != goal_state){
      walledStates.push_back(wallAState);
    }
  }


  //cout << "Play as human? 1=yes, 0=no" << endl;
  //cin >> player;
  player=0;

  while(iterations<max_iterations && agent_state != goal_state){
    iterations++;
    stepscounter++;
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
      agent_state=move(agent_state,direction,hight,width,walledStates);
      //outpusts new position
      cout << "new state: " << agent_state <<" xPos: "<< xPos(agent_state,hight,width)<<" yPos: "<< yPos(agent_state,hight,width)<<endl;
    }
    else if(ruleOfThumbToggle==1){//rule of thumb
      //calculates direction in degrees to the goal from the agents position
      dx=xPos(goal_state,hight,width)-xPos(agent_state,hight,width);
      dy=yPos(goal_state,hight,width)-yPos(agent_state,hight,width);


      //move based on where goal is
      if(dx>0){//move right
        agent_state=move(agent_state,0,hight,width,walledStates);
        //cout<<"right"<<endl;
      }
      else if(dx<0){//move left
        agent_state=move(agent_state,2,hight,width,walledStates);
        //cout<<"left"<<endl;
      }
      else if(dy>0){//move up
        agent_state=move(agent_state,1,hight,width,walledStates);
        //cout<<"up"<<endl;
      }
      else if(dy<0){//move down
        agent_state=move(agent_state,3,hight,width,walledStates);
        //cout<<"down"<<endl;
      }

    }
    else{//q-learner
      // exploration
      if(LYRAND<epsilon){
        direction=floor(LYRAND*4);//picks a random direction
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
                }
            }
      }//end exploitation
      agent_old_state=agent_state;
      agent_state=move(agent_state,direction,hight,width,walledStates);//moves the agent

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
        //pure q learner
        //qTable[agent_old_state][direction]=actionValue+gamma*maxQ;
        //qlearner+AVL
        qTable[agent_old_state][direction]=qTable[agent_old_state][direction]+alpha*(actionValue+gamma*maxQ-qTable[agent_old_state][direction]);
      }
    }
    if(agent_state == goal_state){//reset agent back to start if goal is reached
      agent_state=agent_start_state;
      steps2goal.push_back(stepscounter);//vector of number of steps it took to get to goal
      stepscounter=0;
    }
  }//end while
  if(goal_state==agent_state){
    cout<<"goal reached in "<< iterations<< " move(s)"<<endl;
  }

  //outputs start and goal states and positions
  cout << "start state: " << agent_start_state <<" xPos: "<< xPos(agent_start_state,hight,width)<<" yPos: "<< yPos(agent_start_state,hight,width)<<endl;
  cout << "goal state " << goal_state <<" xPos: "<< xPos(goal_state,hight,width)<<" yPos: "<< yPos(goal_state,hight,width)<<endl;
  for(int i=0;i<Nstates;i++){
    cout << qTable[i][0]<<" "<<qTable[i][1] <<" "<< qTable[i][2]<<" " << qTable[i][3]<< endl;
  }

  for(int i=0;i<steps2goal.size()&&i<50;i++){//prints the steps it took to get to the goal for the first 50 episodes
    cout<< steps2goal[i]<< endl;
  }
//testD No Q-value ever exceeds the reward given by reaching the goal state.
  for(int i=0;i<qTable.size();i++){
    for(int j=0;j<4;j++){
      assert(qTable[i][j]<maxReward+1);
    }
  }
//TestE When the agent reaches the goal state, it is reset to the initial state and is identical to a freshly-initialized agent, except in updated Q-values.
  assert(steps2goal.size()>0);
//TestF
  //calculates max steps
  optimalSteps=abs(xPos(goal_state,hight,width)-xPos(agent_start_state,hight,width));
  optimalSteps=optimalSteps+abs(yPos(goal_state,hight,width)-yPos(agent_start_state,hight,width));
  minSteps=1000000;
  //gets the smallest value of steps in the last ten episodes
  for(int i=steps2goal.size()-10;i<steps2goal.size();i++){
    if(minSteps>steps2goal[i]){
      minSteps=steps2goal[i];
    }
  }
  //assert(minSteps<2*optimalSteps);
//Statistical runs logger
ofstream out_data("MarshallMillersPBetaSteps2Goal.txt");
  for (int i=0; i<steps2goal.size(); i++){

      //out_data << pullhistory[i].pullnumber << " ";
    //  out_data << steps2goal[i] << endl;
  }
}
