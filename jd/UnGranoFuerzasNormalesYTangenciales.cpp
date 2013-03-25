// Suma de Fuerzas sobre un grano, con sólo fuerzas normales
#include <iostream>
#include <cmath>
#include <vector>
u#include"Random.h"
sing namespace std;

const int Zeta=3;
const double alphax=1.0;
const double g60=M_PI/6,double g240=2*M_PI/3;

class Grano{
private:
  double theta[Zeta],double Fn[Zeta],double Ft[Zeta];
public:
  void GenerePosiciones(Crandom & ran2);
  void GenereFuerzas(double alpha,Crandom & ran2);
  double GetFn(void){return Fn;};

};
void Grano::GenerePosiciones(Crandom & ran2){
  theta[0]=0;
  int i;
  for(i=0;i<Zeta;i++)
    theta[i]=theta[i-1]+g60+ran2.r()*(g240-theta[i]);
}
void Grano::GenereFuerzas(double Alpha,Crandom & ran2){
  double dp;
  Fnnew=Fn+a*(2*ran2.r()-1);
  if(Fnnew>0){
    dp=2*(Fnnew-Fn);
    if(dp<0)
      Fn=Fnnew;
    else if(exp(-Alpha*dp)<ran2.r())
      Fn=Fnnew;
  }
}

int main(){
  Grano Pepa;
  Crandom ran2(0);

  Pepa.GenerePosiciones(ran2);
  
  return 0;
}
