// Dos fuerzas normales, diametralmente opuestas, sobre un grano
#include <iostream>
#include <cmath>
#include <vector>
#include"Random.h"
using namespace std;

const double Alpha=1;
const double a=0.1;

int main(){
  Crandom ran2(0);
  double Fn,Fnnew,dp;
  int n;

  Fn=1;
  for(n=0;n<1000;n++){
    do{
      Fnnew=Fn+a*(2*ran2.r()-1);
    }while(Fnnew<0);
    dp=2*(Fnnew-Fn);
    if(dp<0)
      Fn=Fnnew;
    else if(exp(-Alpha*dp)<ran2.r())
      Fn=Fnnew;
    cout<<n<<" "<<Fnnew<<endl;
  }
  
  return 0;
}
