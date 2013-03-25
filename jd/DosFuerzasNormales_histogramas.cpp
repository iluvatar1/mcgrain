// Dos fuerzas normales, diametralmente opuestas, sobre un grano
#include <iostream>
#include <cmath>
#include <vector>
#include"Random.h"
using namespace std;

const double a=0.1;

class Fuerza{
private:
  double Fn,Fnnew;
public:
  void Inicie(void){Fn=1;};
  void MonteCarloStep(double alpha,Crandom & ran2);
  double GetFn(void){return Fn;};
};
void Fuerza::MonteCarloStep(double Alpha,Crandom & ran2){
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

const int teq=2000;
const int Ndatos=10000;

int main(){
  Fuerza MyF;
  Crandom ran2(0);
  double Alpha=0.1;
  int t;

  //Inicializar
  MyF.Inicie();
  //Equilibrar
  for(t=0;t<teq;t++)
    MyF.MonteCarloStep(Alpha,ran2);
  //Imprimir Datos 
  for(t=0;t<Ndatos;t++){
    MyF.MonteCarloStep(Alpha,ran2);
    cout<<MyF.GetFn()<<endl;
  }

  return 0;
}
