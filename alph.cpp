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
    int mu;
    int sigma;
    void init();
};
void armm::init(){
    mu=0;
    sigma=0;
}
int main(){

    srand(time(NULL));
    int mu;
    int sigma;
    int ans;
    int NArms;

    vector<armm> MrManyArms;


    mu =10;
    sigma=1;
    NArms= 10;


    std::random_device rd;
    for (int i=0; i<NArms;i++){
      armm c;
      c.init();
      c.mu=1;
      c.sigma=2;
      MrManyArms.push_back(c);
    }



    std::mt19937 e2(rd());

    //ans= std::normal_distribution<< dist(mu, sigma);

    std::cout << "aids" << std::endl;
    for (int i=0; i<MrManyArms.size(); i++){
            //cout << alldecks[i].decklocation << " ";
            cout << MrManyArms[i].mu << " ";
            cout << MrManyArms[i].sigma << endl;
    }

}
